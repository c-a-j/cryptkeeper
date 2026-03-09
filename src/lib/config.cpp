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
#include "global.hpp"
#include "util/logger.hpp"

inline constexpr std::string_view GLOBAL_CONFIGS = "global";

namespace ck::lib::config {
  using namespace ck::types;
  namespace fs = std::filesystem;
  using namespace ck::util::logger;
  using namespace ck::util::error;
  
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
    std::filesystem::create_directories(dir, ec);
    if (ec) {
      throw Error{ConfigErrc::CreateDirectoryFailed, std::string(dir) };
    }
  }
  
  void parse_key(ConfigKey& key) {
    std::string part;
    std::vector<std::string> key_parts;
    std::stringstream ss(key.key);
    
    while (std::getline(ss, part, '.')) {
      key_parts.push_back(part);
    }
    
    // when user specifies vault (ex -v vault auto_push true)
    if (key.vault && key_parts.size() == 1) {
      key.scope = "vaults";
      key.field = key_parts[0];
    // when user does not specify vault (ex vaults.vault.auto_push true)
    } else if (!key.vault && key_parts.size() == 3) {
      key.scope = key_parts[0];
      key.vault = key_parts[1];
      key.field = key_parts[2];
    // when user modifies global settings (ex global.auto_push true)
    } else if (!key.vault && key_parts.size() == 2) {
      key.scope = key_parts[0];
      key.field = key_parts[1];
    } else {
      ss.clear();
      ss << "scope = " << key.scope << ", vault = " << key.vault.value_or("") << 
        ", field = " << key.field << "\n";
      throw Error{ConfigErrc::InvalidSetParameter, ss.str()};
    }
  }
  
  void print_config_ln(std::string key, std::string value, std::string vault = {}) {
    if (vault.empty()) {
      std::cout << "[global." << key << "] = " << value << "\n";
      return;
    }
    std::cout << "[vaults." << vault << "." << key << "] = " << value << "\n";
  }
  
  void print_str_fields(const VaultConfig& obj, const std::string& vault = {}) {
    for (auto& [key, member] : VaultConfig::str_fields()) {
      if (obj.*member) print_config_ln(std::string(key), *(obj.*member), vault);
    }
  }
  
  void print_bool_fields(const VaultConfig& obj, const std::string& vault = {}) {
    for (auto& [key, member] : VaultConfig::bool_fields()) {
      if (obj.*member) print_config_ln(std::string(key), *(obj.*member) ? "true" : "false", vault);
    }
  }
  
  void print_config(Config& cfg, Vault& vault) {
    if (!vault.name) {
      print_str_fields(cfg.global);
      print_bool_fields(cfg.global);
    }
    
    for (const auto& [v, ov] : cfg.overrides) {
      if (vault.name && v != *vault.name) continue;
      print_str_fields(ov, v);
      print_bool_fields(ov, v);
    }
  }
  
  void print_parameter(Config& cfg, Vault& vault, std::string key) {
    ConfigKey cfg_key;
    cfg_key.key = key;
    cfg_key.vault = vault.name;
    parse_key(cfg_key);
    if (cfg_key.scope == "global") {
      VaultConfig& obj = cfg.global;
      for (auto& [k, member] : VaultConfig::str_fields()) {
        if (k == cfg_key.field) {
          if (obj.*member) print_config_ln(std::string(k), *(obj.*member));
          return;
        }
      }
      for (auto& [k, member] : VaultConfig::bool_fields()) {
        if (k == cfg_key.field) {
          if (obj.*member) print_config_ln(std::string(k), *(obj.*member) ? "true" : "false");
          return;
        }
      }
    }
    
    if (cfg_key.scope == "vaults") {
      for (auto& [v, ov] : cfg.overrides) {
        if (v != cfg_key.vault) continue;
        for (auto& [k, member] : VaultConfig::str_fields()) {
          if (k == cfg_key.field) { 
            if (ov.*member) print_config_ln(std::string(k), *(ov.*member), *cfg_key.vault);
            return; 
          }
        }
        for (auto& [k, member] : VaultConfig::bool_fields()) {
          if (k == cfg_key.field) { 
            if (ov.*member) print_config_ln(std::string(k), *(ov.*member) ? "true" : "false", *cfg_key.vault);
            return; 
          }
        }
      }
    }
  }
  
  toml::table struct_to_toml(Config& cfg) {
    toml::table cfg_toml;
    VaultConfig& obj = cfg.global;
    
    cfg_toml.insert("global", toml::table{});
    toml::table* global = cfg_toml[GLOBAL_CONFIGS].as_table();
    for (auto& [key, member] : VaultConfig::str_fields()) {
      if (obj.*member) global -> insert(key, *(obj.*member));
    }
    for (auto& [key, member] : VaultConfig::bool_fields()) {
      if (obj.*member) global -> insert(key, *(obj.*member));;
    }
    
    cfg_toml.insert("vaults", toml::table{});
    toml::table* vaults = cfg_toml["vaults"].as_table();
    for (const auto& [v, ov] : cfg.overrides) {
      toml::table tbl;
      for (auto& [key, member] : VaultConfig::str_fields()) {
        if (ov.*member) tbl.insert(key, *(ov.*member));
      }
      for (auto& [key, member] : VaultConfig::bool_fields()) {
        if (ov.*member) tbl.insert(key, *(ov.*member));;
      }
      vaults -> insert(v, tbl);
    }
    return cfg_toml;
  }
  
  
  void set_parameter(Config& cfg, Vault& vault, std::vector<std::string> set_args) {
    if (set_args.size() < 2) return;
    const std::string& key = set_args[0];
    const std::string& val = set_args[1];
    
    ConfigKey cfg_key;
    cfg_key.key = key;
    cfg_key.vault = vault.name;
    parse_key(cfg_key);
    
    if (cfg_key.scope == "global") {
      VaultConfig& obj = cfg.global;
      for (auto& [k, member] : VaultConfig::str_fields()) {
        if (k == cfg_key.field) { obj.*member = val; return; }
      }
      for (auto& [k, member] : VaultConfig::bool_fields()) {
        if (k == cfg_key.field) { obj.*member = (val == "true"); return; }
      }
    } else if (cfg_key.scope == "vaults") {
      for (auto& [v, ov] : cfg.overrides) {
        if (v != cfg_key.vault) continue;
        for (auto& [k, member] : VaultConfig::str_fields()) {
          if (k == cfg_key.field) { ov.*member = val; return; }
        }
        for (auto& [k, member] : VaultConfig::bool_fields()) {
          if (k == cfg_key.field) { ov.*member = (val == "true"); return; }
        }
      }
    } else {
      throw Error{ConfigErrc::InvalidSetParameter, cfg_key.scope};
    }
  }
  
  void save_config(Config& cfg) {
    toml::table cfg_toml = struct_to_toml(cfg);
    fs::path cfg_file = app_config_file();
    std::ofstream out(cfg_file, std::ios::out | std::ios::trunc);
    out << cfg_toml << "\n";
    if (!out) {
      throw Error{ConfigErrc::SaveConfigFailed, std::string(cfg_file)};
    }
  }
  
  void create_default_config(Config& cfg, Vault& vault) {
    VaultConfig& obj = cfg.global;
    obj.vault = *vault.name;
    obj.directory = vault_root();
    obj.auto_push = false;
    cfg.overrides[*vault.name] = VaultConfig{};
  }
  
  void init_config(Config& cfg, Vault& vault) {
    fs::path cfg_file = app_config_file();
    // add new vault if config file already exists
    if (fs::exists(cfg_file)) {
      load_config(cfg);
      if (vault.name) cfg.overrides[*vault.name] = VaultConfig{};
      save_config(cfg);
      return;
    }
    // create new config
    create_config_dir();
    create_default_config(cfg, vault);
    save_config(cfg);
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
    
    // parse global configurations
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
  }
}