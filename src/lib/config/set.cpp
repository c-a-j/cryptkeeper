#include <string>
#include <vector>

#include "lib/config/types.hpp"
#include "cli/types.hpp"
#include "./_internal/parse_key.hpp"
#include "util/error.hpp"

namespace ck::config {
  using ck::util::error::Error;
  using ck::util::error::ConfigErrc;
  using enum ck::util::error::ConfigErrc;
  
  static std::optional<bool> parse_bool(std::string_view v) {
    if (v == "true") return true;
    if (v == "false") return false;
    return std::nullopt;
  }
  
  static const VaultConfig::Field* find_field(std::string_view key) {
    for (const auto& f : VaultConfig::fields()) {
      if (f.key == key) return &f;
    }
    return nullptr;
  }
  
  static VaultConfig* resolve_target(Config& cfg, const ConfigKey& key) {
    if (key.scope == "global") return &cfg.global;
    if (key.scope == "vaults" && key.vault) {
      auto it = cfg.overrides.find(*key.vault);
      if (it != cfg.overrides.end()) return &it->second;
    }
    return nullptr;
  }
  
  static void set_field(VaultConfig& obj, const VaultConfig::Field& field, std::string_view val) {
    std::visit([&](auto member) {
      using M = decltype(member);
      if constexpr (std::is_same_v<M, VaultConfig::StrMember>) {
        obj.*member = std::string(val);
      } else {
        auto b = parse_bool(val);
        if (!b) {
          std::string s = std::string(field.key) + " = " + std::string(val);
          throw Error<ConfigErrc>{InvalidSetParameter, s};
        }
        obj.*member = *b;
      }
    }, field.member);
  }
  
  void set_parameter(Config& cfg, const ck::cli::ConfigArgs& args) {
    if (args.set_args.size() < 2) return;
    const std::string& key = args.set_args[0];
    const std::string& val = args.set_args[1];
    
    ConfigKey cfg_key;
    cfg_key.key = key;
    cfg_key.vault = args.vault_name;
    parse_key(cfg_key);
    
    VaultConfig* target = resolve_target(cfg, cfg_key);
    const auto* field = find_field(*cfg_key.field);
    if (!target || !field) {
      throw Error<ConfigErrc>{InvalidSetParameter, *cfg_key.scope};
    }
    set_field(*target, *field, val);
  }
}
