#pragma once
#include <string>
#include <optional>
#include "lib/config/types.hpp"

namespace ck::config {
  struct ConfigKey {
    std::optional<std::string> key;
    std::optional<std::string> scope;
    std::optional<std::string> vault;
    std::optional<std::string> field;
    
    explicit operator bool() const { return !key; }
  };

  void parse_key(ConfigKey& key);
}
