#include <string>
#include <filesystem>
#include <cstdlib>

#include "util/error.hpp"
#include "util/logger/logger.hpp"
#include "lib/types.hpp"
#include "global.hpp"

inline constexpr std::string_view GLOBAL_CONFIGS = "global";

// path functions
namespace ck::config {
  namespace fs = std::filesystem;
  using ck::util::error::Error;
  using ck::util::error::ConfigErrc;
  using enum ck::util::error::ConfigErrc;
  using ck::util::logger::logger;

  static std::string env_or_empty(const char* name) {
    if (const char* value = std::getenv(name)) return value;
    return {};
  }

  fs::path vault_root() {
    std::string vault_dir = env_or_empty(VAULT_DIR_ENV_VAR.data());
    if (!vault_dir.empty()) return fs::path(vault_dir);
    
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

  fs::path app_config_dir() {
    std::string cfg_dir = env_or_empty(CONFIG_DIR_ENV_VAR.data());
    if (!cfg_dir.empty()) return fs::path(cfg_dir);
    
    #ifdef _WIN32
      std::string appdata = env_or_empty("APPDATA");
      if (appdata.empty()) throw std::runtime_error("APPDATA is not set");
      return fs::path(appdata);
    #else
      std::string xdg = env_or_empty("XDG_CONFIG_HOME");
      if (!xdg.empty()) return fs::path(xdg) / APP_DIR;
      
      auto home = env_or_empty("HOME");
      if (home.empty()) throw std::runtime_error("HOME is not set");
      return fs::path(home) / ".config" / APP_DIR;
    #endif
  }
  
  fs::path app_config_file() {
    return app_config_dir() / "config.toml";
  }

  void create_config_dir() {
    fs::path dir = app_config_dir();
    std::error_code ec;
    bool created = std::filesystem::create_directories(dir, ec);
    if (ec) {
      throw Error<ConfigErrc>{CreateDirectoryFailed, std::string(dir) };
    }
    if (created) {
      logger.info("Created new config directory", std::string(dir));
    }
  }
}
