#include <array>
#include <cstdint>
#include <string>
#include <cerrno>
#include <cstring>
#include <format>
#include <ranges>


#include "util/error.hpp"
#include "lib/types.hpp"

#if defined(__linux__)
  #include <sys/random.h>
#elif defined(__APPLE__)
  #include <Security/Security.h>
#elif defined(_WIN32)
  #include <bcrypt.h>
#endif

namespace {
using ck::util::error::Error;
using ck::util::error::CryptoErrc;
using enum ck::util::error::CryptoErrc;
  void fill_random_bytes(std::uint8_t* out, std::size_t len) {
  #if defined(__linux__) 
    std::size_t filled = 0;
    
    while (filled < len) {
      ssize_t nread = getrandom(out + filled, len - filled, 0);
      if (nread > 0) { filled += std::size_t(nread); continue; }
      
      if (nread == -1) { 
        if (errno == EINTR) { continue; }
        throw Error<CryptoErrc>{FillRandomBytesFailed, std::strerror(errno) };
      } 
      throw Error<CryptoErrc>{FillRandomBytesFailed, "getrandom returned 0" };
    }
  #elif defined(__APPLE__)
    if (SecRandomCopyBytes(kSecRandomDefault, len, out) != errSecSuccess) {
      throw Error<CryptoErrc>{FillRandomBytesFailed, "SecRandomCopyBytes failed" };
    }
  #elif defined(_WIN32)
    NTSTATUS status = BCryptGenRandom(
      nullptr,
      reinterpret_cast<PUCHAR>(out),
      static_cast<ULONG>(len),
      BCRYPT_USE_SYSTEM_PREFERRED_RNG
    );
  
    if (!BCRYPT_SUCCESS(status)) {
      throw Error<CryptoErrc>{FillRandomBytesFailed, "BCryptGenRandom failed"};
    }
  #endif
  }
  
  std::array<std::uint8_t, 16> random_bytes_16() {
    std::array<std::uint8_t, 16> bytes{};
    fill_random_bytes(bytes.data(), bytes.size());
    return bytes;
  }
}

namespace ck::crypto {
  using ck::util::error::Error;
  using ck::util::error::CryptoErrc;
  using enum ck::util::error::CryptoErrc;
  std::string uuid_v4() {
    std::string uuid;
    uuid.reserve(36);
    std::array<std::uint8_t, 16> bytes = random_bytes_16();
    bytes[6] = (bytes[6] & 0x0F) | 0x40;
    bytes[8] = (bytes[8] & 0x3F) | 0x80;
    
    for (auto [i, e] : std::views::enumerate(bytes)) {
      uuid += std::format("{:02x}", e);
      if (i == 3 || i == 5 || i == 7 || i == 9) { uuid += '-';}
    }
    return uuid;
  }
  
  std::string get_random_chars(const std::string_view charset, const size_t length) {
    if (length == 0) return {};
    std::string s;
    constexpr std::size_t max_byte = 256;
    const std::size_t limit = max_byte - (max_byte % charset.size());
    
    while (s.size() < length) {
      std::uint8_t b;
      fill_random_bytes(&b, 1);
      
      if (b >= limit) { continue; }
      
      s += charset[b % charset.size()];
    }
    return s;
  }
  
  std::size_t random_index(std::size_t upper_bound) {
    constexpr std::size_t max_byte = 256;
    const std::size_t limit = max_byte - (max_byte % upper_bound);
    
    while (true) {
      std::uint8_t b;
      fill_random_bytes(&b, 1);
      
      if (b < limit) { return b % upper_bound; }
    }
  }
  
  void shuffle(std::string& s) {
    for (std::size_t i = s.size(); i > 1; --i) {
      std::size_t j = random_index(i);
      std::swap(s[i-1], s[j]);
    }
  }
  
  std::string pwgen(PwSpec& spec) {
    std::size_t length, nlow, nupp, nnum, nsym, nrest;
    std::string pass;
    static constexpr std::string_view uppset = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    static constexpr std::string_view lowset = "abcdefghijklmnopqrstuvwxyz";
    static constexpr std::string_view numset = "0123456789";
    static constexpr std::string_view symset = "!@#$%^&*()-_=+[]{};:,.<>?";
    static constexpr std::string_view charset =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz"
      "0123456789"
      "!@#$%^&*()-_=+[]{};:,.<>?";
    
    if (!spec.length) { throw Error<CryptoErrc>{InvalidPwSpec, "a length is required"}; }
    length = *spec.length;
    if (length < 1) { throw Error<CryptoErrc>{InvalidPwSpec, "length must be greater than 0"}; }
    nupp = spec.nupp ? *spec.nupp : 0;
    nlow = spec.nlow ? *spec.nlow : 0;
    nnum = spec.nnum ? *spec.nnum : 0;
    nsym = spec.nsym ? *spec.nsym : 0;
    if (length < nupp + nlow + nnum + nsym) { 
      throw Error<CryptoErrc>{InvalidPwSpec, "length must be greater than sum of parts"}; 
    }
    nrest = length - (nupp + nlow + nnum + nsym);
   
    pass += get_random_chars(uppset, nupp);
    pass += get_random_chars(lowset, nlow);
    pass += get_random_chars(numset, nnum);
    pass += get_random_chars(symset, nsym);
    pass += get_random_chars(charset, nrest);
    shuffle(pass);
    
    return pass;
  }
}