#include <fstream>
#include <filesystem>

#include "lib/types.hpp"
#include "util/error.hpp"

namespace ck::crypto {
  using namespace ck::config;
  using namespace ck::secret;
  using ck::util::error::Error;
  using ck::util::error::CryptoErrc;
  using enum ck::util::error::CryptoErrc;
  namespace fs = std::filesystem;
  
  std::string fingerprint(const Vault& vault, const Secret& secret) {
    if (secret.key_fpr) { return *secret.key_fpr; }
    if (vault.key_fpr) { return *vault.key_fpr; }
    fs::path key_path = fs::path(*vault.directory) / ".gpgid";
    std::ifstream file(key_path);
    if (!file.is_open()) {
      throw Error<CryptoErrc>{GpgIdFileNotFound, std::string(key_path)};
    }
    std::string fingerprint;
    std::getline(file, fingerprint);
    return fingerprint;
  } 
}