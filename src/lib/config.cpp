#include "lib/config.hpp"
#include "toml++/toml.hpp"
#include "lib/types.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <cstdlib>

#include "cmd/config.hpp"
#include "global.hpp"
#include "util/logger.hpp"

namespace ck::lib {
  using namespace ck::types;
  namespace fs = std::filesystem;
  using namespace ck::util::logger;
  
  static std::string env_or_empty(const char* name) {
    if (const char* value = std::getenv(name)) return value;
    return {};
  }

  fs::path config_root() {
  #ifdef _WIN32
    auto appdata = env_or_empty("APPDATA");
    if (appdata.empty()) throw std::runtime_error("APPDATA is not set");
    return fs::path(appdata);
  #else
    auto xdg = env_or_empty("XDG_CONFIG_HOME");
    if (!xdg.empty()) return fs::path(xdg);
    
    auto home = env_or_empty("HOME");
    if (home.empty()) throw std::runtime_error("HOME is not set");
    return fs::path(home) / ".config";
  #endif
  }
  
  fs::path app_config_dir() {
    return config_root() / APP_DIR;
  }
  
  fs::path app_config_file() {
    return app_config_dir() / "config.toml";
  }
  
  void create_config_dir() {
    fs::path dir = app_config_dir();
    std::error_code ec;
    std::filesystem::create_directories(dir, ec);
    if (ec) {
        logger.error("Failed to create config dir: ", std::string(dir)); 
        logger.error(ec.message());
    }
  }
  
  std::string print_defaults() {
    std::ostringstream ss;
    ss << CONFIG_HEADER << "\n"
      << "default-store = \"valult\"" << "\n"
      << "auto-push = false" << "\n"
      << "vault-directory = \"\"" << "\n";
    return ss.str();
  }
  
  void create_config_file() {
    fs::path cfg = app_config_file();
    if (fs::exists(cfg)) return;
    
    std::ofstream out(cfg, std::ios::out | std::ios::trunc);
    out << print_defaults() ; 
    if (!out) {
      logger.error("Failed to create config file: ", std::string(cfg));
      return;
    }
    out.close();
  }
  
  void load_config(Config& config) {
    fs::path cfg_file = app_config_file();
    auto cfg = toml::parse_file("config.toml");
    const std::string vault_from_cli = config.vault;
    
    // global defaults
    if (auto* defaults = cfg["default"].as_table()) {
      config.vault = (*defaults)["directory"].value_or("");
      config.directory = (*defaults)["directory"].value_or("");
      config.auto_push = (*defaults)["auto-push"].value_or(false);
    }
    
    // named vault overrides: [vault.any-name]
    if (auto* vaults = cfg["vaults"].as_table()){
      for (auto&& [k,n] : *vaults) {
        auto* v = n.as_table();
        if (!v) continue;
        
        ConfigOverrides ov{};
        ov.auto_push = (*v)["auto_push"].value<bool>();
        ov.directory = (*v)["vault-directory"].value<std::string>();
        
        config.overrides[std::string(k.str())] = std::move(ov);
      }
    }
    
    if (!vault_from_cli.empty()) {
      auto it = config.overrides.find(vault_from_cli);
      if (it != config.overrides.end()) {
        const auto& ov = it->second;
        if (ov.auto_push) config.auto_push = *ov.auto_push;
        if (ov.directory) config.directory = *ov.directory;
      }
    }
  }
}