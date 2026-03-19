# pragma once

#include <string>
#include <optional>
#include <unordered_map>
#include <array>
#include <vector>
#include <variant>
#include <filesystem>

namespace ck::config {
  using namespace std::string_view_literals;
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

namespace ck::config::refactor {
  namespace fs = std::filesystem;
  
  template<typename Owner, typename Member>
  using MemberPtr = Member Owner::*;

  template<typename T>
  struct Field {
    std::string_view key;
    T member;
  };

  struct Core {
    bool autopush = true;
    bool autosync = true;

    using StrMember = MemberPtr<Core, std::string>;
    using BoolMember = MemberPtr<Core, bool>;
    using FieldVariant = Field<std::variant<StrMember, BoolMember>>; 
    
    inline static constexpr std::array<FieldVariant, 2> k_fields {{
        {"autopush", &Core::autopush},
        {"autosync", &Core::autosync}
      }};
    
    static constexpr const std::array<FieldVariant, 2>& fields() { return k_fields; }
    static constexpr const std::string_view k_name = "core";
  };

  struct Ui {
    bool insert_with_editor = false;
    
    using StrMember = MemberPtr<Ui, std::string>;
    using BoolMember = MemberPtr<Ui, bool>;
    using FieldVariant = Field<std::variant<StrMember, BoolMember>>; 
    
    inline static constexpr std::array<FieldVariant, 1> k_fields {{
        {"insert_with_editor", &Ui::insert_with_editor},
      }};
    
    static constexpr const std::array<FieldVariant, 1>& fields() { return k_fields; }
    static constexpr const std::string_view k_name = "ui";
  };

  struct Pwgen {
    int length = 20;
    int symbols = 5;
    int numbers = 5;
    int uppercase = 5;
    int lowercase = 5;
    
    using IntMember = MemberPtr<Pwgen, int>;
    using FieldVariant = Field<std::variant<IntMember>>; 
    
    inline static constexpr std::array<FieldVariant, 5> k_fields {{
        {"length", &Pwgen::length},
        {"symbols", &Pwgen::symbols},
        {"numbers", &Pwgen::numbers},
        {"uppercase", &Pwgen::uppercase},
        {"lowercase", &Pwgen::lowercase},
      }};
    
    static constexpr const std::array<FieldVariant, 5>& fields() { return k_fields; }
    static constexpr const std::string_view k_name = "pwgen";
  };

  class Config {
    public:
      explicit Config();
      
      const Core& core() const;
      Core& core();
      
      const Ui& ui() const;
      Ui& ui();
      
      const Pwgen& pwgen() const;
      Pwgen& pwgen();
      
      template <typename Fn>
      bool with_section(std::string_view name, Fn&& fn) {
        for (const auto& section : sections()) {
          if (section.name != name) {
            continue;
          }
          std::visit([&](auto member) {
            std::forward<Fn>(fn)(this->*member);
          }, section.member);
          return true;
        }
        return false;
      }
      
      template <typename Fn>
      bool with_member(std::string_view section_name, std::string_view member_name, Fn&& fn) {
        bool found = false;
        
        auto outer_fn = [&](auto& section){
          using Owner = std::remove_cvref_t<decltype(section)>;
          
          for (const auto& field : Owner::fields())  {
            if (field.key != member_name) { continue; }
            
            std::visit([&](auto& member) {
              (fn)(section.*member);
            }, field.member);
            found = true;
            return;
          }
        };
        
        with_section(section_name, outer_fn);
        return found;
      }
      
      void print();
      void print(std::string_view);
      void set(std::vector<std::string>);
      void write();

    private:
      Core core_;
      Ui ui_;
      Pwgen pwgen_;
      void deserialize();
      
      using SectionMember = std::variant<
        Core Config::*,
        Ui Config::*,
        Pwgen Config::*
      >;
      
      struct Section {
        std::string_view name;
        SectionMember member;
      };
      
      inline static constexpr std::array<Section, 3> k_sections {{
        {Core::k_name, &Config::core_},
        {Ui::k_name, &Config::ui_},
        {Pwgen::k_name, &Config::pwgen_},
      }};
      
      static constexpr const std::array<Section, 3>& sections() { return k_sections; }
  };
}


