#pragma once
#include <filesystem>

#include "types.hpp"

namespace ck::lib::config {
using namespace ck::types;
  struct ConfigKey {
    std::string key;
    std::string scope;
    std::optional<std::string> vault;
    std::string field;
    
    explicit operator bool() const { return !key.empty(); }
  };
  
  std::filesystem::path vault_root();
  void init_config(Config&, Vault&);
  void load_config(Config&);
  void print_config(Config&, Vault&);
  void print_parameter(Config&, Vault&, const std::string = {});
  void set_parameter(Config&, Vault&, std::vector<std::string>);
  void save_config(Config&);
}
