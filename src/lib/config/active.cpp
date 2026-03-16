#include "lib/types.hpp"

#include "lib/config/deserialize.hpp"

namespace ck::config {
  
  void apply_overrides(VaultConfig& active, const VaultConfig& overrides) {
    for (const auto& field : VaultConfig::fields()) {
      std::visit([&](auto member) {
        const auto& v = overrides.*member;
        if (v) active.*member = v;
      }, field.member);
    }
  }
  
  VaultConfig get_active_config(Config& cfg, VaultConfig& active, Vault& vault) {
    config::deserialize(cfg);
    
    active = cfg.global;
    
    if (!vault.name) { return active; }
    active.vault = vault.name;
    
    if (auto it = cfg.overrides.find(*vault.name); it != cfg.overrides.end()) {
      apply_overrides(active, it->second);
    }
    
    vault.directory = active.directory;
    
    return active;
  }
}