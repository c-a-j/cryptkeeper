#include "toml++/toml.hpp"

#include "util/error.hpp"
#include "lib/config/types.hpp"
#include "util/logger/logger.hpp"

namespace {
  using namespace ck::config;
  using ck::util::error::Error;
  using ck::util::error::ConfigErrc;
  using enum ck::util::error::ConfigErrc;
  using ck::util::logger::logger;

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
  using ck::util::error::Error;
  using ck::util::error::ConfigErrc;
  using enum ck::util::error::ConfigErrc;
  using ck::util::logger::logger;
  
  toml::table serialize(const State& state) {
    toml::table tbl;
    
    insert_table(state.core, tbl);
    insert_table(state.ui, tbl);
    insert_table(state.pwgen, tbl);
    return tbl;
  }
}
