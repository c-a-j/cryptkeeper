#include <array>
#include <cstdint>
#include <string>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <sstream>
#include <format>
#include <ranges>


#include "util/error.hpp"

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
}