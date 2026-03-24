#include <string>
#include <filesystem>
#include <cstdlib>

#include "util/error.hpp"
#include "util/logger/logger.hpp"
#include "global.hpp"

// path functions
namespace ck::path {
  namespace fs = std::filesystem;
  using ck::util::error::Error;
  using ck::util::error::PathErrc;
  using enum ck::util::error::PathErrc;
  using ck::util::logger::logger;

  static std::string env_or_empty(const char* name) {
    if (const char* value = std::getenv(name)) return value;
    return {};
  }

  fs::path vault_root() {
    std::string vault_dir = env_or_empty(VAULT_DIR_ENV_VAR.data());
    if (!vault_dir.empty()) return fs::path(vault_dir);
    
    #ifdef _WIN32
      std::string appdata = env_or_empty("LOCALAPPDATA");
      if (appdata.empty()) {
        throw Error<PathErrc>{LocalAppDataNotSet}; 
      }
      return fs::path(appdata) / APP_DIR / COLLECTION_HOME_DIR;
    #else
      auto xdg_data = env_or_empty("XDG_DATA_HOME");
      if (!xdg_data.empty()) return fs::path(xdg_data);
      
      auto home = env_or_empty("HOME");
      if (home.empty()) { 
        throw Error<PathErrc>{DataHomeNotSet}; 
      }
      return fs::path(home) / ".local" / "share" / APP_DIR;
    #endif
  }

  fs::path config_dir() {
    std::string cfg_dir = env_or_empty(CONFIG_DIR_ENV_VAR.data());
    if (!cfg_dir.empty()) return fs::path(cfg_dir);
    
    #ifdef _WIN32
      std::string appdata = env_or_empty("LOCALAPPDATA");
      if (appdata.empty()) {
        throw Error<PathErrc>{LocalAppDataNotSet}; 
      }
      return fs::path(appdata) / APP_DIR / fs::path("config");
    #else
      std::string xdg = env_or_empty("XDG_CONFIG_HOME");
      if (!xdg.empty()) return fs::path(xdg) / APP_DIR;
      
      auto home = env_or_empty("HOME");
      if (home.empty()) { 
        throw Error<PathErrc>{HomeNotSet}; 
      }
      return fs::path(home) / ".config" / APP_DIR;
    #endif
  }
  
  fs::path config_file() {
    return config_dir() / CONFIG_FILE;
  }

  fs::path mount_dir() {
    return config_dir();
  }

  fs::path mount_file() {
    return config_dir() / MOUNT_FILE;
  }

  void create_config_dir() {
    fs::path dir = ck::path::config_dir();
    std::error_code ec;
    bool created = std::filesystem::create_directories(dir, ec);
    if (ec) {
      throw Error<PathErrc>{CreateDirectoryFailed, dir.string() };
    }
    if (created) {
      logger.info("Created new config directory", dir.string());
    }
  }
}
