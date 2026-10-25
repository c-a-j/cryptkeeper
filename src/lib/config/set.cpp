#include "error.hpp"
#include "internal/parse_key.hpp"
#include "lib/config/config.hpp"
#include "ui/console/console.hpp"

using ck::config::cfg;

namespace {
using namespace ck::config;
using ck::error::ConfigErrc;
using ck::error::Error;
using enum ck::error::ConfigErrc;
using ck::ui::console::console;

int parse_int(
    const std::string &section,
    const std::string &key,
    const std::string &value
) {
    std::size_t pos = 0;
    int out;
    try {
        out = std::stoi(value, &pos);
    } catch (const std::invalid_argument &) {
        throw Error<ConfigErrc>{
            InvalidConfigValue, section + "." + key + " = " + value};
    } catch (const std::out_of_range &) {
        throw Error<ConfigErrc>{
            InvalidConfigValue, section + "." + key + " = " + value};
    }

    if (pos != value.size()) {
        throw Error<ConfigErrc>{
            InvalidConfigValue, section + "." + key + " = " + value};
    }

    if (out < 0) {
        throw Error<ConfigErrc>{
            InvalidConfigValue, section + "." + key + " = " + value};
    }

    return out;
}

bool set_field(Ui &cfg, std::string_view field, std::string_view value) {
    if (field == "insert_with_editor") {
        cfg.insert_with_editor = (value == "true");
        return true;
    }
    return false;
}

bool set_field(Core &cfg, std::string_view field, std::string_view value) {
    if (field == "home") {
        cfg.home = value;
        return true;
    }
    if (field == "autopush") {
        cfg.autopush = (value == "true");
        return true;
    }
    if (field == "autosync") {
        cfg.autosync = (value == "true");
        return true;
    }
    return false;
}

bool set_field(Pwgen &cfg, const std::string &field, const std::string &value) {
    const std::string section = "pwgen";
    if (field == "length") {
        cfg.length = parse_int(section, field, value);
        return true;
    }
    if (field == "numbers") {
        cfg.numbers = parse_int(section, field, value);
        return true;
    }
    if (field == "uppercase") {
        cfg.uppercase = parse_int(section, field, value);
        return true;
    }
    if (field == "lowercase") {
        cfg.lowercase = parse_int(section, field, value);
        return true;
    }
    if (field == "symbols") {
        cfg.symbols = parse_int(section, field, value);
        return true;
    }
    return false;
}
} // namespace

namespace ck::config {
using ck::error::ConfigErrc;
using ck::error::Error;
using enum ck::error::ConfigErrc;
using ck::ui::console::console;

void Config::set(std::vector<std::string> args) {
    std::vector<std::string> key_parts = parse_key(args[0]);
    const std::string &value = args[1];

    if (key_parts.size() != 2) {
        throw Error<ConfigErrc>{InvalidConfigKey, std::string(args[0])};
    }

    State state = this->state_;
    bool found = false;

    if (key_parts[0] == "core") {
        found = set_field(state.core, key_parts[1], value);
    }
    if (key_parts[0] == "ui") {
        found = set_field(state.ui, key_parts[1], value);
    }
    if (key_parts[0] == "pwgen") {
        found = set_field(state.pwgen, key_parts[1], value);
    }

    if (!found) {
        throw Error<ConfigErrc>{InvalidConfigKey, std::string(args[0])};
    }

    // if pwgen setup isn't valid, throw
    if (auto result = state.valid_pwgen(); !result) {
        throw result.error();
    }

    this->state_ = state;
}
} // namespace ck::config
