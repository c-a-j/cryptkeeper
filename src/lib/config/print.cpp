#include <iostream>

#include "error.hpp"
#include "internal/parse_key.hpp"
#include "lib/config/config.hpp"
#include "ui/console/console.hpp"

namespace {
using namespace ck::config;
using ck::error::ConfigErrc;
using ck::error::Error;
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
    std::cout << "[" << section << "." << key
              << "] = " << (value ? "true" : "false") << "\n";
}

void print_section(const CoreCfg &cfg) {
    std::string section = "core";
    print_ln(section, "home", cfg.home);
    print_ln(section, "autopush", cfg.autopush);
    print_ln(section, "autosync", cfg.autosync);
}

void print_section(const UiCfg &cfg) {
    std::string section = "ui";
    print_ln(section, "insert_with_editor", cfg.insert_with_editor);
}

void print_section(const PwgenCfg &cfg) {
    std::string section = "pwgen";
    print_ln(section, "length", cfg.length);
    print_ln(section, "symbols", cfg.symbols);
    print_ln(section, "numbers", cfg.numbers);
    print_ln(section, "uppercase", cfg.uppercase);
    print_ln(section, "lowercase", cfg.lowercase);
}

bool print_field(const CoreCfg &cfg, std::string_view key) {
    if (key == "home") {
        print_ln("core", key, cfg.home);
    } else if (key == "autopush") {
        print_ln("core", key, cfg.autopush);
    } else if (key == "autosync") {
        print_ln("core", key, cfg.autosync);
    } else {
        return false;
    }
    return true;
}

bool print_field(const UiCfg &cfg, std::string_view key) {
    if (key == "insert_with_editor") {
        print_ln("ui", key, cfg.insert_with_editor);
    } else {
        return false;
    }
    return true;
}

bool print_field(const PwgenCfg &cfg, std::string_view key) {
    if (key == "length") {
        print_ln("pwgen", key, cfg.length);
    } else if (key == "numbers") {
        print_ln("pwgen", key, cfg.numbers);
    } else if (key == "symbols") {
        print_ln("pwgen", key, cfg.symbols);
    } else if (key == "lowercase") {
        print_ln("pwgen", key, cfg.lowercase);
    } else if (key == "uppercase") {
        print_ln("pwgen", key, cfg.uppercase);
    } else {
        return false;
    }
    return true;
}
} // namespace

namespace ck::config {
using ck::error::ConfigErrc;
using ck::error::Error;
using enum ck::error::ConfigErrc;
using ck::ui::console::console;


void print_cfg(const Config &cfg) {
    print_section(cfg.core);
    print_section(cfg.ui);
    print_section(cfg.pwgen);
}

void print_cfg(const Config &cfg, std::string_view key) {
    std::vector<std::string> key_parts = parse_key(key);
    bool found = false;

    if (key_parts.size() == 1) {
        const std::string &section = key_parts[0];

        if (section == "core") {
            print_section(cfg.core);
            found = true;
        } else if (section == "ui") {
            print_section(cfg.ui);
            found = true;
        } else if (section == "pwgen") {
            print_section(cfg.pwgen);
            found = true;
        }
    }

    if (key_parts.size() == 2) {
        const std::string &section = key_parts[0];
        const std::string &field = key_parts[1];

        if (section == "core") {
            found = print_field(cfg.core, field);
        } else if (section == "ui") {
            found = print_field(cfg.ui, field);
        } else if (section == "pwgen") {
            found = print_field(cfg.pwgen, field);
        }
    }

    if (!found) {
        throw Error<ConfigErrc>{InvalidConfigKey, std::string(key)};
    }
}

} // namespace ck::config
