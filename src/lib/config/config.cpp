#include "lib/config/config.hpp"

#include "../path/internal/path.hpp"
#include "error.hpp"

namespace ck::config {
using ck::error::ConfigErrc;
using ck::error::Error;
using enum ck::error::ConfigErrc;

const std::string &home_dir(const Config &cfg) {
    if (cfg.core.home.empty()) {
        throw Error<ConfigErrc>{
            InvalidConfigFile, "core.home must be defined without --path "};
    }
    return cfg.core.home;
}

std::string &home_dir(Config &cfg) {
    if (cfg.core.home.empty()) {
        cfg.core.home = ck::path::crypt_root();
    }
    return cfg.core.home;
}

} // namespace ck::config
