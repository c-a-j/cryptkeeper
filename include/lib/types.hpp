# pragma once
#include <string>
#include <optional>
#include <unordered_map>
#include <vector>
#include <array>
#include <tuple>
#include <variant>


namespace ck::types {
  using namespace std::string_view_literals;
  struct Vault {
    std::optional<std::string> name;
    std::optional<std::string> key_fpr;
    std::optional<std::string> directory;
  };
  
  struct Secret {
    std::string path;
    std::string key_fpr;
  };

  struct VaultConfig {
    std::optional<std::string> vault;
    std::optional<std::string> directory;
    std::optional<bool> auto_push;

    using StrMember = std::optional<std::string> VaultConfig::*;
    using BoolMember = std::optional<bool> VaultConfig::*;
    
    struct Field {
      std::string_view key;
      std::variant<StrMember, BoolMember> member;
    };
    
    inline static constexpr std::array<Field, 3> k_fields {{
        {"vault", &VaultConfig::vault},
        {"directory", &VaultConfig::directory},
        {"auto_push", &VaultConfig::auto_push},
      }};
    
    static constexpr const std::array<Field, 3>& fields() { return k_fields; }
    
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
