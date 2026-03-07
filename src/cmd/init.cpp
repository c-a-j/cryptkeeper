#include <string>
#include <filesystem>
#include <fstream>

#include "cmd/init.hpp"
#include "global.hpp"
#include "util/logger.hpp"
#include "lib/crypto.hpp"
#include "lib/config.hpp"
#include "util/error.hpp"



namespace ck::cmd::init {
  namespace fs = std::filesystem;
  using namespace ck::util::logger;
  using namespace ck::util::error;
    
  static std::string env_or_empty(const char* name) {
    if (const char* value = std::getenv(name)) return value;
    return {};
  }
  
  fs::path store_root() {
    std::string cfg_dir = env_or_empty(VAULT_DIR_ENV_VAR.data());
    if (!cfg_dir.empty()) return fs::path(cfg_dir);
    
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
  
  void init_vault(std::string store_name, std::string key_fpr) {
    if (!ck::lib::crypto::public_key_exists(key_fpr)) {
      throw Error{InitErrc::KeyNotFound, key_fpr};
    }
    
    fs::path dir = store_root() / store_name;
    
    std::error_code ec;
    bool created = fs::create_directories(dir, ec);
    if (ec) {
      throw Error{InitErrc::CreateDirectoryFailed, ec.message()};
    }
    
    if (!created) {
      throw Error{InitErrc::AlreadyExists, dir};
    } 
    
    const fs::path gpg_id_path = dir / ".gpg-id";
    std::ofstream gpg_id_file(gpg_id_path, std::ios::out | std::ios::trunc);
    if (!gpg_id_file.is_open()) {
      throw Error{InitErrc::OpenGpgIdFailed, std::string(gpg_id_path)};
    }
    
    gpg_id_file << key_fpr << '\n';
    if (!gpg_id_file) {
      throw Error{InitErrc::WriteGpgIdFailed, std::string(gpg_id_path)};
    }
  }
  
}
