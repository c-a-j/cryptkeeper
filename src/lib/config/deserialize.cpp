#include "toml++/toml.hpp"

#include "error.hpp"
#include "lib/config/config.hpp"
#include "ui/console/console.hpp"

namespace {
using namespace ck::config;
using ck::error::ConfigErrc;
using ck::error::Error;
using enum ck::error::ConfigErrc;
using ck::ui::console::console;

void parse_default_warning(std::string_view section, std::string_view field) {
    std::string msg1 = "Parameter [" + std::string(section) + "." +
        std::string(field) + "] contains an invalid data type";
    std::string msg2 = "default will be applied";
    console.warning(msg1, msg2);
}

void missing_section_warning(std::string_view section) {
    std::string msg1 =
        "Section " + std::string(section) + " is missing from config file";
    std::string msg2 = "defaults will be applied";
    console.warning(msg1, msg2);
}

void parse_fields(UiCfg &cfg, const toml::table &tbl, std::string_view section) {
    std::string field = "insert_with_editor";
    if (auto val = tbl[field].value<bool>()) {
        cfg.insert_with_editor = *val;
    } else {
        parse_default_warning(section, field);
    }
}

void parse_fields(
    PwgenCfg &cfg,
    const toml::table &tbl,
    std::string_view section
) {
    std::string field = "length";
    if (auto val = tbl[field].value<int>()) {
        cfg.length = *val;
    } else {
        parse_default_warning(section, field);
    }

    field = "symbols";
    if (auto val = tbl[field].value<int>()) {
        cfg.symbols = *val;
    } else {
        parse_default_warning(section, field);
    }

    field = "numbers";
    if (auto val = tbl[field].value<int>()) {
        cfg.numbers = *val;
    } else {
        parse_default_warning(section, field);
    }

    field = "uppercase";
    if (auto val = tbl[field].value<int>()) {
        cfg.uppercase = *val;
    } else {
        parse_default_warning(section, field);
    }

    field = "lowercase";
    if (auto val = tbl[field].value<int>()) {
        cfg.lowercase = *val;
    } else {
        parse_default_warning(section, field);
    }
}

void parse_fields(CoreCfg &cfg, const toml::table &tbl, std::string_view section) {
    std::string field = "home";
    if (auto val = tbl[field].value<std::string>()) {
        cfg.home = *val;
    } else {
        parse_default_warning(section, field);
    }

    field = "autopush";
    if (auto val = tbl[field].value<bool>()) {
        cfg.autopush = *val;
    } else {
        parse_default_warning(section, field);
    }

    field = "autosync";
    if (auto val = tbl[field].value<bool>()) {
        cfg.autosync = *val;
    } else {
        parse_default_warning(section, field);
    }
}

void valid_or_default(PwgenCfg &pwgen) {
    if (auto result = validate_pwgen(pwgen); !result) {
        console.error(result.error().msg1(), result.error().msg2());
        PwgenCfg def;
        pwgen = def;
    }
}
} // namespace

namespace ck::config::codec {
using ck::error::ConfigErrc;
using ck::error::Error;
using enum ck::error::ConfigErrc;
using ck::ui::console::console;

Config deserialize(std::string_view toml_text) {
    toml::table cfg_toml;
    try {
        cfg_toml = toml::parse(toml_text);
    } catch (const toml::parse_error &e) {
        throw Error<ConfigErrc>{
            InvalidConfigFile, std::string(e.description())};
    }

    Config cfg_tmp;

    std::string section = "core";
    if (auto *tbl = cfg_toml[section].as_table()) {
        parse_fields(cfg_tmp.core, *tbl, section);
    } else {
        missing_section_warning(section);
    }

    section = "ui";
    if (auto *tbl = cfg_toml[section].as_table()) {
        parse_fields(cfg_tmp.ui, *tbl, section);
    } else {
        missing_section_warning(section);
    }

    section = "pwgen";
    if (auto *tbl = cfg_toml[section].as_table()) {
        parse_fields(cfg_tmp.pwgen, *tbl, section);
    } else {
        missing_section_warning(section);
    }
    valid_or_default(cfg_tmp.pwgen);

    return cfg_tmp;
}
} // namespace ck::config::codec
