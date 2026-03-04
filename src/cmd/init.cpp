#include <string>
#include <filesystem>
#include <fstream>

#include "cmd/init.hpp"
#include "global.hpp"
#include "util/logger.hpp"
#include "lib/crypto.hpp"



namespace ck::cmd::init {
  namespace fs = std::filesystem;
  using namespace ck::util::logger;
    
  static std::string env_or_empty(const char* name) {
    if (const char* value = std::getenv(name)) return value;
    return {};
  }
  
  fs::path store_root() {
  #ifdef _WIN32
    auto appdata = env_or_empty("APPDATA");
    if (appdata.empty()) throw std::runtime_error("APPDATA is not set");
    return fs::path(appdata) / APP_NAME;
  #else
    auto xdg_data = env_or_empty("XDG_DATA_HOME");
    if (!xdg_data.empty()) return fs::path(xdg_data);
    
    auto home = env_or_empty("HOME");
    if (home.empty()) throw std::runtime_error("HOME is not set");
    return fs::path(home) / ".local" / "share" / APP_DIR;
  #endif
  }
  
  std::expected<void, InitError> error(InitErrc code, std::string msg1, std::string msg2 = {}) {
    if (msg2.empty()) {
      logger.error(msg1);
    } else {
      logger.error(msg1, msg2);
    }
    return std::unexpected(InitError{code, msg1, msg2});
  }
  
  std::expected<void, InitError> create_store(std::string store_name, std::string key_fpr) {
    if (!ck::lib::crypto::public_key_exists(key_fpr)) {
      return error(InitErrc::KeyNotFound, "Public key not found: ", key_fpr);
    }
    
    fs::path dir = store_root() / store_name;
    
    std::error_code ec;
    bool created = fs::create_directories(dir, ec);
    if (ec) {
      return error(InitErrc::CreateDirectoryFailed, "Failed to create crypt: " + ec.message());
    }
    
    if (!created) {
      return error(InitErrc::AlreadyExists, "Crypt already exists: ", dir);
    } 
    
    const fs::path gpg_id_path = dir / ".gpg-id";
    std::ofstream gpg_id_file(gpg_id_path, std::ios::out | std::ios::trunc);
    if (!gpg_id_file.is_open()) {
      return error(InitErrc::OpenGpgIdFailed, "Failed to open .gpg-id for writing: ");
    }
    
    gpg_id_file << key_fpr << '\n';
    if (!gpg_id_file) {
      return error(InitErrc::WriteGpgIdFailed, "Failed to write .gpg-id: ");
    }
    
    return {};
  }
  
  int init_store(std::string store_name, std::string store_key) {
    std::expected<void, InitError> r = create_store(store_name, store_key);
    if (!r) { return 1; }
    logger.success("Store initialized: ", store_name);
    return 0;
  }
}
