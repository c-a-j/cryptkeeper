#include "toml++/toml.hpp"

#include "lib/config/config.hpp"

namespace ck::config::codec {
  toml::table serialize(const State& state) {
    toml::table tbl;
    
    tbl.insert("core", toml::table{
      {"home", state.core.home},
      {"autopush", state.core.autopush},
      {"autosync", state.core.autosync},
    });

    tbl.insert("ui", toml::table{
      {"insert_with_editor", state.ui.insert_with_editor},
    });

    tbl.insert("pwgen", toml::table{
      {"length", state.pwgen.length},
      {"symbols", state.pwgen.symbols},
      {"numbers", state.pwgen.numbers},
      {"uppercase", state.pwgen.uppercase},
      {"lowercase", state.pwgen.lowercase},
    });

    return tbl;
  }
}
