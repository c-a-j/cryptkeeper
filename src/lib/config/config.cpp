#include "toml++/toml.hpp"

#include <filesystem>
#include <iostream>

#include "util/error.hpp"
#include "lib/config/types.hpp"
#include "./_internal/path.hpp"
#include "util/logger/logger.hpp"


namespace {
  using ck::util::error::Error;
  using ck::util::error::ConfigErrc;
  using enum ck::util::error::ConfigErrc;
  using ck::util::logger::logger;
  
  template <typename T>
  struct member_value;

  template <typename Owner, typename Value>
  struct member_value<Value Owner::*> {
    using type = Value;
  };

  template <typename T>
  using member_value_t = typename member_value<T>::type;

  template <typename Owner>
  void parse_fields(Owner& cfg, const toml::table& tbl) {
    for (const auto& field : Owner::fields()) {
      std::visit([&](auto member){
        using member_ptr_t = decltype(member);
        using value_t = member_value_t<member_ptr_t>;

        if (auto value = tbl[field.key].template value<value_t>()) {
          cfg.*member = *value;
        }
      }, field.member);
    }
  }

  template <typename Owner>
  void insert_fields(const Owner& cfg, toml::table& tbl) {
    for (const auto& field : Owner::fields())  {
      std::visit([&](auto member){
        tbl.insert_or_assign(field.key, cfg.*member);
      }, field.member);
    }
  }
  
  template <typename T>
  void insert_table(const T& cfg, toml::table& tbl) {
    tbl.insert(T::k_name, toml::table{});
    toml::table* subtbl = tbl[T::k_name].as_table();
    insert_fields(cfg, *subtbl);
  }
  
  toml::table serialize(const ck::config::refactor::Config& cfg) {
    toml::table tbl;
    
    insert_table(cfg.core(), tbl);
    insert_table(cfg.ui(), tbl);
    insert_table(cfg.pwgen(), tbl);
    return tbl;
  }

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


namespace ck::config::refactor {
  using ck::util::error::Error;
  using ck::util::error::ConfigErrc;
  using enum ck::util::error::ConfigErrc;
  using ck::util::logger::logger;
  
  Config::Config() { deserialize(); }
  
  const Core& Config::core() const { return core_; }
  Core& Config::core() { return core_; }
  
  const Ui& Config::ui() const { return ui_; }
  Ui& Config::ui() { return ui_; }
  
  const Pwgen& Config::pwgen() const { return pwgen_; }
  Pwgen& Config::pwgen() { return pwgen_; }
  
  void Config::deserialize() {
    fs::path path = app_config_file();
    if (!fs::exists(path)) {
      return;
    }
    
    try {
      auto cfg_toml = toml::parse_file(std::string(path));
      for (auto& section : this->sections()) {
        std::visit([&](auto& member) {
          if (auto* tbl = cfg_toml[section.name].as_table()) {
            parse_fields(this->*member, *tbl);
          }
        }, section.member);
      }
    } catch (const toml::parse_error& e) {
      throw Error<ConfigErrc>{InvalidConfigFile, std::string(e.description())};
    }
  }
  
  void Config::print() {
    for (const auto& section : this->sections()) {
      std::visit([&](auto m){
        print_section(this->*m);
      }, section.member);
    }
  }
  
  void Config::print(std::string_view key) {
    std::vector<std::string> key_parts = parse_key(key);
    bool found = false;
    
    if (key_parts.size() == 1) {
      found = with_section(key_parts[0], [&](const auto& section){
        print_section(section);
      });
    } else if (key_parts.size() == 2) {
      found = with_member(key_parts[0], key_parts[1], [&](const auto& member){
        print_ln(key_parts[0], key_parts[1], member);
      });
    }
    if (!found) {
      throw Error<ConfigErrc>{InvalidConfigKey, std::string(key)};
    }
  }
  
  void Config::set(std::vector<std::string> args) {
    std::vector<std::string> key_parts = parse_key(args[0]);
    std::string value = args[1];
    if (key_parts.size() != 2) {
      throw Error<ConfigErrc>{InvalidConfigKey, std::string(args[0])};
    }
    with_member(key_parts[0], key_parts[1], [&](auto& member){
      using T = std::remove_cvref_t<decltype(member)>;
      if constexpr (std::is_same_v<T, std::string>) {
        member = value;
      } else if constexpr (std::is_same_v<T, bool>) {
        member = (value == "true");
      } else if constexpr (std::is_same_v<T, int>) {
        member = std::stoi(value);
      }
    });
  }
  
  void Config::write() {
    create_config_dir();  
    toml::table tbl = serialize(*this);
    fs::path cfg_file = app_config_file();
    bool exists = fs::exists(cfg_file);
    std::ofstream out(cfg_file, std::ios::out | std::ios::trunc);
    out << tbl << "\n";
    if (!out) {
      throw Error<ConfigErrc>{SaveConfigFailed, std::string(cfg_file)};
    }
    if (!exists) {
      logger.info("Created new config file", std::string(cfg_file));
    } else {
      logger.info("Updated config file", std::string(cfg_file));
    }
  }
  
}
