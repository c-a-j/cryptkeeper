#include "lib/config.hpp"
#include "toml++/toml.hpp"
#include "lib/types.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <cstdlib>
#include <tuple>

#include "cmd/config.hpp"
#include "util/error.hpp"
#include "lib/default_config.hpp"
#include "global.hpp"
#include "util/logger.hpp"

inline constexpr std::string_view GLOBAL_CONFIGS = "global";

namespace ck::lib::config {
  using namespace ck::types;
  namespace fs = std::filesystem;
  using namespace ck::util::logger;
  
  static std::string env_or_empty(const char* name) {
    if (const char* value = std::getenv(name)) return value;
    return {};
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
    std::filesystem::create_directories(dir, ec);
    if (ec) {
        logger.error("Failed to create config dir: ", std::string(dir)); 
        logger.error(ec.message());
    }
  }
  
  void print_default() {
    std::cout << DEFAULT_CONFIG << std::endl;
  }
  
  void print_config_ln(std::string key, std::string value, std::string vault = {}) {
    if (vault.empty()) {
      std::cout << "[global." << key << "] = " << value << "\n";
      return;
    }
    std::cout << "[vaults." << vault << "." << key << "] = " << value << "\n";
  }
  
  void print_str_fields(const VaultConfig& obj, const std::string& vault = {}) {
    for (auto& [key, member] : VaultConfig::str_fields())
      if (obj.*member) print_config_ln(std::string(key), *(obj.*member), vault);
  }
  void print_bool_fields(const VaultConfig& obj, const std::string& vault = {}) {
    for (auto& [key, member] : VaultConfig::bool_fields())
      if (obj.*member) print_config_ln(std::string(key), *(obj.*member) ? "true" : "false", vault);
  }
  void print_config(Config& cfg, Vault& vault) {
    if (vault.name.empty()) {
      print_str_fields(cfg.global);
      print_bool_fields(cfg.global);
    }
    
    for (const auto& [v, ov] : cfg.overrides) {
      if (!vault.name.empty() && v != vault.name) continue;
      print_str_fields(ov, v);
      print_bool_fields(ov, v);
    }
  }
  
  void set_parameter(Config& cfg, Vault& vault, std::vector<std::string> set_args) {
    fs::path cfg_file = app_config_file();
    auto cfg_toml = toml::parse_file(std::string(cfg_file));
  }
  
  void create_default_config_file() {
    fs::path cfg = app_config_file();
    if (fs::exists(cfg)) return;
    
    std::ofstream out(cfg, std::ios::out | std::ios::trunc);
    out << DEFAULT_CONFIG ; 
    if (!out) {
      logger.error("Failed to create config file: ", std::string(cfg));
      return;
    }
    out.close();
  }
  
  void init_config_file(std::string vault, std::string vault_dir) {
    fs::path cfg = app_config_file();
    if (!fs::exists(cfg)) {
      logger.error("Config file doesn't exist: ", std::string(cfg));
      return;
    }
    auto tbl = toml::parse_file(cfg.string());
    tbl[GLOBAL_CONFIGS].as_table() -> insert_or_assign("vault", vault);
    tbl["directory"].as_table() -> insert_or_assign("directory", vault_dir);
  }
  
  void load_str_fields(VaultConfig& obj, const toml::table& tbl){
    for (auto& [key, member] : VaultConfig::str_fields())
      obj.*member = tbl[key].value<std::string>();
  }
  void load_bool_fields(VaultConfig& obj, const toml::table& tbl){
    for (auto& [key, member] : VaultConfig::bool_fields())
      obj.*member = tbl[key].value<bool>();
  }
  
  void load_config(Config& cfg) {
    fs::path cfg_file = app_config_file();
    if (!fs::exists(cfg_file)) {
      logger.error("Config file not found: ", std::string(cfg_file));
      return;
    }
    auto cfg_toml = toml::parse_file(std::string(cfg_file));
    std::string vault_from_cli;
    if (cfg.global.vault) { vault_from_cli = *cfg.global.vault; }
    
    // parse global defaults
    if (auto* globals = cfg_toml[GLOBAL_CONFIGS].as_table()) {
      load_str_fields(cfg.global, *globals);
      load_bool_fields(cfg.global, *globals);
    }
    
    // parse named vault overrides: [vault.any-name]
    if (auto* vaults = cfg_toml["vaults"].as_table()){
      for (auto&& [k,n] : *vaults) {
        auto* v = n.as_table();
        if (!v) continue;
        
        VaultConfig ov{};
        load_str_fields(ov, *v);
        load_bool_fields(ov, *v);
        cfg.overrides[std::string(k.str())] = std::move(ov);
      }
    }
   
    // if a specific vault is specified, apply it's overrides
    if (!vault_from_cli.empty()) {
      auto it = cfg.overrides.find(vault_from_cli);
      if (it != cfg.overrides.end()) {
        const auto& ov = it->second;
        for (auto& [key, member] : VaultConfig::str_fields())
          if (ov.*member) cfg.global.*member = *(ov.*member);
        for (auto& [key, member] : VaultConfig::bool_fields())
          if (ov.*member) cfg.global.*member = *(ov.*member);
      }
    }

  }
}