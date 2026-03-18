#include <optional>

#include "lib/config/types.hpp"
#include "cli/types.hpp"
#include "lib/config/deserialize.hpp"

namespace ck::config {
 using namespace ck::cli; 
  void apply_overrides(VaultConfig& active, const VaultConfig& overrides) {
    for (const auto& field : VaultConfig::fields()) {
      std::visit([&](auto member) {
        const auto& v = overrides.*member;
        if (v) active.*member = v;
      }, field.member);
    }
  }
  
  VaultConfig get_active_config(Config& cfg, VaultConfig& acfg, const CmdArgs& cargs) {
    config::deserialize(cfg);
    acfg = cfg.global;

    std::optional<std::string> vault_name = 
      std::visit([](const auto& args) -> std::optional<std::string> {
        using T = std::decay_t<decltype(args)>;
  
        if constexpr (std::is_same_v<T, std::monostate>) {
          return std::nullopt;
        } else if constexpr (std::is_same_v<T, MountArgs>) {
          return std::nullopt;
        } else if constexpr (std::is_same_v<T, UmountArgs>) {
          return std::nullopt;
        } else {
          return args.vault_name;
        }
      }, cargs);
    
    if (!vault_name) { return acfg; }
    acfg.vault = vault_name;
    
    if (auto it = cfg.overrides.find(*vault_name); it != cfg.overrides.end()) {
      apply_overrides(acfg, it->second);
    }
    
    return acfg;
  }
}
