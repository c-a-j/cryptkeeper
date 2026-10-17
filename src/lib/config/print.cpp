#include "toml++/toml.hpp"

#include <sstream>
#include <iostream>

#include "error.hpp"
#include "lib/config/config.hpp"
#include "ui/console/console.hpp"

namespace {
  using namespace ck::config;
  using ck::error::Error;
  using ck::error::ConfigErrc;
  using enum ck::error::ConfigErrc;
  using ck::ui::console::console;

  void print_ln(
    const std::string_view section,
    const std::string_view key,
    const std::string_view value
  ) {
    std::cout << "[" << section << "." << key << "] = " << value << "\n";
  }

  void print_ln(
    const std::string_view section,
    const std::string_view key,
    const int value
  ) {
    std::cout << "[" << section << "." << key << "] = " << value << "\n";
  }

  void print_ln(
    const std::string_view section,
    const std::string_view key,
    const bool value
  ) {
    std::cout << "[" << section << "." << key << "] = " 
      << (value  ? "true" : "false") << "\n";
  }

  template <typename Owner>
  void print_section(const Owner& cfg) {
    for (const auto& field : Owner::fields())  {
      std::visit([&](auto member){
        print_ln(Owner::k_name, field.key, cfg.*member);
      }, field.member);
    }
  }

  void print_section(const Core& cfg) {
    std::string section = "core";
    print_ln(section, "home", cfg.home);
    print_ln(section, "autopush", cfg.autopush);
    print_ln(section, "autosync", cfg.autosync);
  }

  void print_section(const Ui& cfg) {
    std::string section = "ui";
    print_ln(section, "insert_with_editor", cfg.insert_with_editor);
  }

  void print_section(const Pwgen& cfg) {
    std::string section = "pwgen";
    print_ln(section, "length", cfg.length);
    print_ln(section, "symbols", cfg.symbols);
    print_ln(section, "numbers", cfg.numbers);
    print_ln(section, "uppercase", cfg.uppercase);
    print_ln(section, "lowercase", cfg.lowercase);
  }
  
  std::vector<std::string> parse_key(std::string_view key) {
    std::string part;
    std::stringstream ss(key.data());
    std::vector<std::string> key_parts;
    key_parts.reserve(2);
    
    while (std::getline(ss, part, '.')) {
      key_parts.push_back(part);
    }
    return key_parts;
  }
}


namespace ck::config {
  using ck::error::Error;
  using ck::error::ConfigErrc;
  using enum ck::error::ConfigErrc;
  using ck::ui::console::console;
  
  void Config::print() const {
    print_section(state_.core);
    print_section(state_.ui);
    print_section(state_.pwgen);
  }
  
  void Config::print(std::string_view key) {
    std::vector<std::string> key_parts = parse_key(key);
    bool found = false;

    State state = this->state_;
    
    if (key_parts.size() == 1) {
      const std::string& section = key_parts[0];

      if (section == "core") {
        print_section(state_.core);
        found = true;
      } else if (section == "ui") {
        print_section(state_.ui);
        found = true;
      } else if (section == "pwgen") {
        print_section(state_.pwgen);
        found = true;
      }
    }

    if (!found) {
      throw Error<ConfigErrc>{InvalidConfigKey, std::string(key)};
    }
  }
}
