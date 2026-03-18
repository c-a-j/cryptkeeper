#include "toml++/toml.hpp"

#include "lib/config/types.hpp"

inline constexpr std::string_view GLOBAL_CONFIGS = "global";

// serialize
namespace ck::config {
  void insert_fields(const VaultConfig& obj, toml::table& tbl) {
    for (const auto& field : VaultConfig::fields()) {
      std::visit([&](auto member) {
        using M = decltype(member);
        if constexpr (std::is_same_v<M, VaultConfig::StrMember>) {
          if (obj.*member) tbl.insert(field.key, *(obj.*member));
        } else {
          if (obj.*member) tbl.insert(field.key, *(obj.*member));
        }
      }, field.member);
    }
  }
  
  toml::table serialize(const Config& cfg) {
    toml::table cfg_toml;
    
    cfg_toml.insert("global", toml::table{});
    toml::table* global = cfg_toml[GLOBAL_CONFIGS].as_table();
    insert_fields(cfg.global, *global);
    
    cfg_toml.insert("vaults", toml::table{});
    toml::table* vaults = cfg_toml["vaults"].as_table();
    for (const auto& [v, ov] : cfg.overrides) {
      toml::table tbl;
      insert_fields(ov, tbl);
      vaults -> insert(v, tbl);
    }
    return cfg_toml;
  }
}