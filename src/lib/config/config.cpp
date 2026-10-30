#include "lib/config/config.hpp"

#include "../path/internal/path.hpp"
#include "error.hpp"

namespace ck::config {
using ck::error::ConfigErrc;
using ck::error::Error;
using enum ck::error::ConfigErrc;

const CoreCfg &Config::core() const {
    return state_.core;
}
CoreCfg &Config::core() {
    return state_.core;
}

const UiCfg &Config::ui() const {
    return state_.ui;
}
UiCfg &Config::ui() {
    return state_.ui;
}

const PwgenCfg &Config::pwgen() const {
    return state_.pwgen;
}
PwgenCfg &Config::pwgen() {
    return state_.pwgen;
}

const std::string &Config::home() const {
    if (state_.core.home.empty()) {
        throw Error<ConfigErrc>{
            InvalidConfigFile, "core.home must be defined without --path "};
    }
    return state_.core.home;
}

std::string &Config::home() {
    if (state_.core.home.empty()) {
        state_.core.home = ck::path::crypt_root();
    }
    return state_.core.home;
}

Config cfg;
} // namespace ck::config
