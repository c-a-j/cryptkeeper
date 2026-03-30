# pragma once

#include <string>
#include <array>
#include <vector>
#include <variant>
#include <expected>

#include "lib/crypto/types.hpp"
#include "util/error.hpp"

namespace ck::config {
  using Err = ck::util::error::Error<ck::util::error::ConfigErrc>;

  template<typename Owner, typename Member>
  using MemberPtr = Member Owner::*;

  template<typename T>
  struct Field {
    std::string_view key;
    T member;
  };

  struct Core {
    std::string home;
    bool autopush = true;
    bool autosync = true;

    using StrMember = MemberPtr<Core, std::string>;
    using BoolMember = MemberPtr<Core, bool>;
    using FieldVariant = Field<std::variant<StrMember, BoolMember>>; 
    
    inline static constexpr std::array<FieldVariant, 3> k_fields {{
        {"home", &Core::home},
        {"autopush", &Core::autopush},
        {"autosync", &Core::autosync}
      }};
    
    static constexpr const std::array<FieldVariant, 3>& fields() { return k_fields; }
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

    std::expected<void, Err> valid();
    ck::crypto::PwSpec spec();
    
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

  struct State {
    Core core;
    Ui ui;
    Pwgen pwgen;

    std::expected<void, Err> valid_pwgen();

    using SectionMember = std::variant<
      Core State::*,
      Ui State::*,
      Pwgen State::*
    >;
    
    struct Section {
      std::string_view name;
      SectionMember member;
    };
    
    inline static constexpr std::array<Section, 3> k_sections {{
      {Core::k_name, &State::core},
      {Ui::k_name, &State::ui},
      {Pwgen::k_name, &State::pwgen},
    }};
    
    inline static constexpr const std::array<Section, 3>& sections() { return k_sections; }

    // I definitely made a convienience for readability trade here. The
    // following two templates reduce the need to add to parsing helpers
    // when new parameters and sections are added to the configuration.
    // If a new parameter is added, only this file will need changes.

    // with_section() makes it convenient to execute a lambda function on a
    // specific section (Core, Ui, etc).

    // with_member() makes it convenient to execute a lambda function on a
    // specific field (member) of a specific configuration section (Core, Ui,
    // etc)

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

    template <typename Fn>
    void with_sections(Fn&& fn) {
      for (const auto& section : sections()) {
        std::visit([&](auto member) {
          std::forward<Fn>(fn)(this->*member);
        }, section.member);
      }
    }

    template <typename Fn>
    void with_members(Fn&& fn) {
      auto outer_fn = [&](auto& section){
        using Owner = std::remove_cvref_t<decltype(section)>;
        
        for (const auto& field : Owner::fields())  {
          std::visit([&](auto& member) {
            (fn)(section.*member);
          }, field.member);
        }
      };
      
      with_sections(outer_fn);
    }
  };

  class Config {
    public:
      explicit Config();

      static Config empty();
      
      const Core& core() const;
      Core& core();
      
      const Ui& ui() const;
      Ui& ui();
      
      const Pwgen& pwgen() const;
      Pwgen& pwgen();
      
      const std::string& home() const;
      std::string& home();
      
      void print();
      void print(std::string_view);
      void set(std::vector<std::string>);

      void load();
      void save();

    private:
      State state_;
  };

  extern Config cfg;
}
