#pragma once

#include <expected>
#include <string>
#include <string_view>
#include <vector>

#include "lib/config/error.hpp"
#include "lib/config/pwgen.hpp"

namespace ck::config {
using Err = ck::error::Error<ck::error::ConfigErrc>;

struct CoreCfg {
    std::string home;
    bool autopush = true;
    bool autosync = true;
};

struct UiCfg {
    bool insert_with_editor = false;
};

std::expected<void, Err> validate_pwgen(const PwgenCfg &cfg);

struct Config {
    CoreCfg core;
    UiCfg ui;
    PwgenCfg pwgen;
};

void print_cfg(const Config &cfg);
void print_cfg(const Config &cfg, std::string_view key);
void set_cfg(Config &cfg, const std::vector<std::string>& args);
void save_cfg(const Config &cfg);
void load_cfg(Config &cfg);

const std::string &home_dir(const Config &cfg);
std::string &home_dir(Config &cfg);

extern Config cfg;
} // namespace ck::config
