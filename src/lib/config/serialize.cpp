#include "toml++/toml.hpp"

#include "error.hpp"
#include "lib/config/config.hpp"
#include "ui/console/console.hpp"

namespace {
  using namespace ck::config;
  using ck::error::Error;
  using ck::error::ConfigErrc;
  using enum ck::error::ConfigErrc;
  using ck::ui::console::console;

  template <typename Owner>
  void insert_fields(const Owner& cfg, toml::table& tbl) {
    for (const auto& field : Owner::fields())  {
      std::visit([&](auto member){
        tbl.insert_or_assign(field.key, cfg.*member);
      }, field.member);
    }
  }
  
  template <typename T>
  void insert_table(const T& cfg, toml::table& tbl) {
    tbl.insert(T::k_name, toml::table{});
    toml::table* subtbl = tbl[T::k_name].as_table();
    insert_fields(cfg, *subtbl);
  }
}

namespace ck::config::codec {
  using namespace ck::config;
  using ck::error::Error;
  using ck::error::ConfigErrc;
  using enum ck::error::ConfigErrc;
  using ck::ui::console::console;
  
  toml::table serialize(const State& state) {
    toml::table tbl;
    
    insert_table(state.core, tbl);
    insert_table(state.ui, tbl);
    insert_table(state.pwgen, tbl);
    return tbl;
  }
}
