# pragma once
#include <string>
#include <optional>
#include <unordered_map>
#include <array>
#include <vector>
#include <variant>
#include <map>


namespace ck::config {
  using namespace std::string_view_literals;
  struct Vault {
    std::optional<std::string> name;
    std::optional<std::string> directory;
    std::optional<std::string> key_fpr;
  };
  
  struct VaultConfig {
    std::optional<std::string> vault;
    std::optional<std::string> directory;
    std::optional<bool> auto_push;
    std::optional<bool> insert_with_editor;

    using StrMember = std::optional<std::string> VaultConfig::*;
    using BoolMember = std::optional<bool> VaultConfig::*;
    
    struct Field {
      std::string_view key;
      std::variant<StrMember, BoolMember> member;
    };
    
    inline static constexpr std::array<Field, 4> k_fields {{
        {"vault", &VaultConfig::vault},
        {"directory", &VaultConfig::directory},
        {"auto_push", &VaultConfig::auto_push},
        {"insert_with_editor", &VaultConfig::insert_with_editor},
      }};
    
    static constexpr const std::array<Field, 4>& fields() { return k_fields; }
  };
  
  struct Config {
    VaultConfig global;
    std::unordered_map<std::string, VaultConfig> overrides;
  };
}

namespace ck::secret {
  struct Secret {
    std::optional<std::string> path;
    std::optional<std::string> value;
    std::optional<std::string> key_fpr;
    bool pwgen = false;
  };
}


namespace ck::crypto {
  struct PwSpec {
    std::optional<std::size_t> length;
    std::optional<std::size_t> nsym;
    std::optional<std::size_t> nnum;
    std::optional<std::size_t> nupp;
    std::optional<std::size_t> nlow;
  };
}