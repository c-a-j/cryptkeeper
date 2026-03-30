#include "toml++/toml.hpp"

#include <filesystem>

#include "util/error.hpp"
#include "lib/config/types.hpp"
#include "util/logger/logger.hpp"

#include "../path/path.hpp"
#include "../path/existence.hpp"

namespace {
  using namespace ck::config;
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

  // visit each field in a section
  // this parsing function is tolerant of a malformed configuration file
  // warning is displayed and default is applied if values are missing or malformed
  // defaults are set in the struct definitions
  template <typename Owner>
  void parse_fields(Owner& cfg, const toml::table& tbl, const std::string_view section) {
    for (const auto& field : Owner::fields()) {
      std::visit([&](auto member){
        using member_ptr_t = decltype(member);
        using value_t = member_value_t<member_ptr_t>;
        if (auto value = tbl[field.key].template value<value_t>()) {
          cfg.*member = *value;
        } else {
          std::string msg1 = "Parameter [" + std::string(section) + "."
            + std::string(field.key) + "] contains an invalid data type";
          std::string msg2 = "default will be applied";
          logger.warning(msg1, msg2);
        }
      }, field.member);
    }
  }

  void valid_or_default(Core& _) {
    return;
  }

  void valid_or_default(Ui& _) {
    return;
  }

  void valid_or_default(Pwgen& pwgen) {
    if (auto result = pwgen.valid(); !result) {
      logger.error(result.error().msg1, result.error().msg2);
      Pwgen def;
      pwgen = def;
    }
    return;
  }

}

namespace ck::config::codec {
  using ck::util::error::Error;
  using ck::util::error::ConfigErrc;
  using enum ck::util::error::ConfigErrc;
  using ck::util::logger::logger;
  
  State deserialize(std::string_view toml_text) {
    std::filesystem::path path = ck::path::config_file();
    
    toml::table cfg_toml;
    try {
      cfg_toml = toml::parse(toml_text);
    } catch (const toml::parse_error& e) {
      throw Error<ConfigErrc>{InvalidConfigFile, std::string(e.description())};
    }

    State state;

    // visit each config file section and parse the values
    // missing sections will display a warning and fall back to defaults
    for (auto& section : state.sections()) {
      std::visit([&](auto& member) {
        if (auto* tbl = cfg_toml[section.name].as_table()) {
          parse_fields(state.*member, *tbl, section.name);
          valid_or_default(state.*member);
        } else {
          std::string msg1 = "Section " + std::string(section.name) + " is missing from config file";
          std::string msg2 = "defaults will be applied";
          logger.warning(msg1, msg2);
        }
      }, section.member);
    }
   return state;
  }
}
