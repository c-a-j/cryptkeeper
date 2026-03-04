# pragma once
#include <string>
#include <optional>
#include <unordered_map>


namespace ck::types {
  struct Vault {
    std::string name;
    std::string key_fpr;
  };
  
  struct Secret {
    std::string path;
    std::string key_fpr;
  };
  
  struct ConfigOverrides {
    std::optional<bool> auto_push;
    std::optional<std::string> directory;
  };
  
  struct Config {
    std::string vault;
    std::string directory;
    bool auto_push;
    std::unordered_map<std::string, ConfigOverrides> overrides;
  };
}
