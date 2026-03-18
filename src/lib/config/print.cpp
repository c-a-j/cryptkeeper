#include <iostream>
#include <string>
#include <optional>

#include "lib/config/types.hpp"
#include "cli/types.hpp"
#include "./_internal/parse_key.hpp"

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
  
  void print_config(const Config& cfg, const ck::cli::ConfigArgs& args) {
    if (!args.vault_name) {
      print_fields(cfg.global, args.vault_name);
    }
    
    for (const auto& [v, ov] : cfg.overrides) {
      if (args.vault_name && v != *args.vault_name) continue;
      print_fields(ov, args.vault_name);
    }
  }
  
  void print_parameter(
    const Config& cfg, 
    const ck::cli::ConfigArgs& args
  ) {
    ConfigKey cfg_key;
    cfg_key.key = args.set_args[0];
    cfg_key.vault = args.vault_name;
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