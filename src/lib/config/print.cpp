#include "toml++/toml.hpp"

#include <sstream>
#include <iostream>

#include "util/error.hpp"
#include "lib/config/types.hpp"
#include "util/logger/logger.hpp"

namespace {
  using namespace ck::config;
  using ck::util::error::Error;
  using ck::util::error::ConfigErrc;
  using enum ck::util::error::ConfigErrc;
  using ck::util::logger::logger;

  template <typename T>
  void print_ln(
      const std::string_view section,
      const std::string_view key,
      const T& value
    ) {
    if constexpr (std::is_same_v<std::remove_cvref_t<T>, bool>) {
      std::cout << "[" << section << "." << key << "] = " << (value  ? "true" : "false");
    } else {
      std::cout << "[" << section << "." << key << "] = " << value;
    }
    std::cout << "\n";
  }

  template <typename Owner>
  void print_section(const Owner& cfg) {
    for (const auto& field : Owner::fields())  {
      std::visit([&](auto member){
        print_ln(Owner::k_name, field.key, cfg.*member);
      }, field.member);
    }
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
  using ck::util::error::Error;
  using ck::util::error::ConfigErrc;
  using enum ck::util::error::ConfigErrc;
  using ck::util::logger::logger;
  
  void Config::print() {
    State state = this->state_;

    for (const auto& section : state.sections()) {
      std::visit([&](auto m){
        print_section(state.*m);
      }, section.member);
    }
  }
  
  void Config::print(std::string_view key) {
    std::vector<std::string> key_parts = parse_key(key);
    bool found = false;

    State state = this->state_;
    
    if (key_parts.size() == 1) {
      found = state.with_section(key_parts[0], [&](const auto& section){
        print_section(section);
      });
    } else if (key_parts.size() == 2) {
      found = state.with_member(key_parts[0], key_parts[1], [&](const auto& member){
        print_ln(key_parts[0], key_parts[1], member);
      });
    }
    if (!found) {
      throw Error<ConfigErrc>{InvalidConfigKey, std::string(key)};
    }
  }
}
