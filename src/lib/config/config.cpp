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

const std::string &home_dir(const State &state) {
    if (state.core.home.empty()) {
        throw Error<ConfigErrc>{
            InvalidConfigFile, "core.home must be defined without --path "};
    }
    return state.core.home;
}

std::string &home_dir(State &state) {
    if (state.core.home.empty()) {
        state.core.home = ck::path::crypt_root();
    }
    return state.core.home;
}

const std::string &Config::home() const {
    return home_dir(state_);
}

std::string &Config::home() {
    return home_dir(state_);
}

Config cfg;
} // namespace ck::config
