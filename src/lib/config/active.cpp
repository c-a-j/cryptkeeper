#include "lib/types.hpp"

namespace ck::lib::config {
  using namespace ck::types;
  
  void apply_overrides(VaultConfig& active, const VaultConfig& overrides) {
    for (const auto& field : VaultConfig::fields()) {
      std::visit([&](auto member) {
        const auto& v = overrides.*member;
        if (v) active.*member = v;
      }, field.member);
    }
  }
  
  VaultConfig get_active_config(const Config& cfg, const Vault& vault) {
    VaultConfig active = cfg.global;
    
    if (!vault.name) { return active; }
    
    if (auto it = cfg.overrides.find(*vault.name); it != cfg.overrides.end()) {
      apply_overrides(active, it->second);
    }
    
    return active;
  }
}