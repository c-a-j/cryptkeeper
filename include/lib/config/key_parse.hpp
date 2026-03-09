#pragma once
#include <string>
#include <optional>

namespace ck::lib::config {
  struct ConfigKey {
    std::optional<std::string> key;
    std::optional<std::string> scope;
    std::optional<std::string> vault;
    std::optional<std::string> field;
    
    explicit operator bool() const { return !key; }
  };

  void parse_key(ConfigKey& key);
}
