#include "toml++/toml.hpp"

#include <sstream>

#include "util/error.hpp"
#include "lib/config/types.hpp"
#include "util/logger/logger.hpp"

using ck::config::cfg;

namespace {
  using namespace ck::config;
  using ck::util::error::Error;
  using ck::util::error::ConfigErrc;
  using enum ck::util::error::ConfigErrc;
  using ck::util::logger::logger;

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
  
  int parse_int(const std::vector<std::string>& key, const std::string& value) {
    std::size_t pos = 0;
    int out;
    try {
      out = std::stoi(value, &pos);
    } catch (const std::invalid_argument&) {
      throw Error<ConfigErrc>{InvalidConfigValue, key[0] + "." + key[1] + " = " + value};
    } catch (const std::out_of_range&) {
      throw Error<ConfigErrc>{InvalidConfigValue, key[0] + "." + key[1] + " = " + value};
    }
    
    if (pos != value.size()) {
      throw Error<ConfigErrc>{InvalidConfigValue, key[0] + "." + key[1] + " = " + value};
    }

    if (out < 0) {
      throw Error<ConfigErrc>{InvalidConfigValue, key[0] + "." + key[1] + " = " + value};
    }
    
    return out;
  }
}


namespace ck::config {
  using ck::util::error::Error;
  using ck::util::error::ConfigErrc;
  using enum ck::util::error::ConfigErrc;
  using ck::util::logger::logger;
  
  
  void Config::set(std::vector<std::string> args) {
    std::vector<std::string> key_parts = parse_key(args[0]);
    std::string value = args[1];
    if (key_parts.size() != 2) {
      throw Error<ConfigErrc>{InvalidConfigKey, std::string(args[0])};
    }
    State state = this->state_;

    bool found = state.with_member(key_parts[0], key_parts[1], [&](auto& member){
      using T = std::remove_cvref_t<decltype(member)>;
      if constexpr (std::is_same_v<T, std::string>) {
        member = value;
      } else if constexpr (std::is_same_v<T, bool>) {
        member = (value == "true");
      } else if constexpr (std::is_same_v<T, int>) {
        member = parse_int(key_parts, value);
      }
    });

    if (!found) {
      throw Error<ConfigErrc>{InvalidConfigKey, std::string(args[0])};
    }

    // if pwgen setup isn't valid, throw
    if (auto result = state.valid_pwgen(); !result) {
      throw result.error();
    }

    this->state_ = state;
  }
}
