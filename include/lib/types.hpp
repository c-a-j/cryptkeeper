# pragma once
#include <string>
#include <optional>
#include <unordered_map>
#include <vector>
#include <array>
#include <tuple>


namespace ck::types {
  using namespace std::string_view_literals;
  struct Vault {
    std::string name;
    std::string key_fpr;
    std::string directory;
  };
  
  struct Secret {
    std::string path;
    std::string key_fpr;
  };

  struct VaultConfig {
    std::optional<std::string> vault;
    std::optional<std::string> directory;
    std::optional<bool> auto_push = false;

    static constexpr auto str_fields() {
      return std::array{
        std::pair{"directory"sv, &VaultConfig::directory},
        std::pair{"vault"sv, &VaultConfig::vault}
      };
    }
    static constexpr auto bool_fields() {
      return std::array{
        std::pair{"auto_push"sv, &VaultConfig::auto_push}
      };
    }
  };
  
  struct Config {
    VaultConfig global;
    std::unordered_map<std::string, VaultConfig> overrides;
  };
}
