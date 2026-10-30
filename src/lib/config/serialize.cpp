#include "toml++/toml.hpp"

#include "lib/config/config.hpp"

namespace ck::config::codec {
toml::table serialize(const Config &cfg) {
    toml::table tbl;

    tbl.insert(
        "core",
        toml::table{
            {"home", cfg.core.home},
            {"autopush", cfg.core.autopush},
            {"autosync", cfg.core.autosync},
        }
    );

    tbl.insert(
        "ui",
        toml::table{
            {"insert_with_editor", cfg.ui.insert_with_editor},
        }
    );

    tbl.insert(
        "pwgen",
        toml::table{
            {"length", cfg.pwgen.length},
            {"symbols", cfg.pwgen.symbols},
            {"numbers", cfg.pwgen.numbers},
            {"uppercase", cfg.pwgen.uppercase},
            {"lowercase", cfg.pwgen.lowercase},
        }
    );

    return tbl;
}
} // namespace ck::config::codec
