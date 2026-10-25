#include "lib/config/config.hpp"
#include "error.hpp"

#include "../path/internal/path.hpp"

namespace ck::config {
using ck::error::ConfigErrc;
using ck::error::Error;
using enum ck::error::ConfigErrc;

const Core &Config::core() const {
    return state_.core;
}
Core &Config::core() {
    return state_.core;
}

const Ui &Config::ui() const {
    return state_.ui;
}
Ui &Config::ui() {
    return state_.ui;
}

const Pwgen &Config::pwgen() const {
    return state_.pwgen;
}
Pwgen &Config::pwgen() {
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
