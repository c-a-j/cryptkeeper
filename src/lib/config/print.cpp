#include "lib/types.hpp"
#include <iostream>
#include <string>
#include <optional>

#include "lib/types.hpp"
#include "lib/config/parse_key.hpp"

namespace ck::config {
  // using namespace ck::util::error;

  void print_config_ln(
    const std::string& key, 
    const std::string& value, 
    const std::optional<std::string>& vault
  ) {
    if (!vault) {
      std::cout << "[global." << key << "] = " << value << "\n";
      return;
    }
    std::cout << "[vaults." << *vault << "." << key << "] = " << value << "\n";
  }
  
  void print_fields(
    const VaultConfig& obj, 
    const std::optional<std::string>& vault,
    const std::optional<std::string>& field = std::nullopt
  ) {
    for (const auto& f : VaultConfig::fields()) {
      if (field && *field != f.key) { continue; }
      std::visit([&](auto member) {
        using M = decltype(member);
        if constexpr (std::is_same_v<M, VaultConfig::StrMember>) {
          if (obj.*member) print_config_ln(std::string(f.key), *(obj.*member), vault);
        } else {
          if (obj.*member) print_config_ln(std::string(f.key), *(obj.*member) ? "true" : "false", vault);
        }
      }, f.member);
    }
  }
  
  void print_config(const Config& cfg, const Vault& vault) {
    if (!vault.name) {
      print_fields(cfg.global, vault.name);
    }
    
    for (const auto& [v, ov] : cfg.overrides) {
      if (vault.name && v != *vault.name) continue;
      print_fields(ov, vault.name);
    }
  }
  
  void print_parameter(
    const Config& cfg, 
    const Vault& vault, 
    const std::string& key
  ) {
    ConfigKey cfg_key;
    cfg_key.key = key;
    cfg_key.vault = vault.name;
    parse_key(cfg_key);

    if (cfg_key.scope == "global") {
      print_fields(cfg.global, cfg_key.vault, cfg_key.field);
    }
    
    if (cfg_key.scope == "vaults") {
      for (auto& [v, ov] : cfg.overrides) {
        if (v != cfg_key.vault) { continue; }
        print_fields(ov, cfg_key.vault, cfg_key.field);
      }
    }
  }
}