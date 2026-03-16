#include "toml++/toml.hpp"
#include <string>
#include <filesystem>

#include "./path.hpp"
#include "./default.hpp"
#include "lib/types.hpp"
#include "util/logger/logger.hpp"

inline constexpr std::string_view GLOBAL_CONFIGS = "global";

namespace ck::config {
  namespace fs = std::filesystem;
  using namespace ck::util::logger;

  void parse_fields(VaultConfig& obj, const toml::table& tbl) {
    for (const auto& field : VaultConfig::fields()) {
      std::visit([&](auto member) {
        using M = decltype(member);
        if constexpr (std::is_same_v<M, VaultConfig::StrMember>) {
          obj.*member = tbl[field.key].value<std::string>();
        } else {
          obj.*member = tbl[field.key].value<bool>();
        }
      }, field.member);
    }
  }
  
  void deserialize(Config& cfg) {
    fs::path cfg_file = app_config_file();
    if (!fs::exists(cfg_file)) {
      logger.info("Config file not found", std::string(cfg_file));
      load_default(cfg);
      return;
    }
    auto cfg_toml = toml::parse_file(std::string(cfg_file));
    
    // parse global configurations
    if (auto* globals = cfg_toml[GLOBAL_CONFIGS].as_table()) {
      parse_fields(cfg.global, *globals);
    }
    
    // parse named vault overrides: [vault.any-name]
    if (auto* vaults = cfg_toml["vaults"].as_table()){
      for (auto&& [k,n] : *vaults) {
        auto* v = n.as_table();
        if (!v) continue;
        
        VaultConfig ov{};
        parse_fields(ov, *v);
        cfg.overrides[std::string(k.str())] = std::move(ov);
      }
    }
  }
}
