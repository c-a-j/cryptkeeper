#include "toml++/toml.hpp"

#include <filesystem>
#include <sstream>

#include "util/error.hpp"
#include "lib/config/types.hpp"
#include "util/logger/logger.hpp"

#include "../fio/atomic_write.hpp"
#include "../path/path.hpp"

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
  
  toml::table serialize(const ck::config::Config& cfg) {
    toml::table tbl;
    
    insert_table(cfg.core(), tbl);
    insert_table(cfg.ui(), tbl);
    insert_table(cfg.pwgen(), tbl);
    return tbl;
  }
}

namespace ck::config {
  using ck::util::error::Error;
  using ck::util::error::ConfigErrc;
  using enum ck::util::error::ConfigErrc;
  using ck::util::logger::logger;
  
  void Config::write() {
    ck::path::create_config_dir();  
    toml::table tbl = serialize(*this);
    std::filesystem::path cfg_file = ck::path::config_file();
    
    std::error_code ec;
    bool existed = std::filesystem::exists(cfg_file, ec);
    if (ec) { existed = false; }
    
    std::ostringstream contents;
    contents << tbl << "\n";
    
    logger.debug("Writing config file");
    ck::fio::atomic_write(cfg_file, contents.str());
    
    if (!existed) {
      logger.info("Created new config file", cfg_file.string());
    } else {
      logger.info("Updated config file", cfg_file.string());
    }
    this->print();
  }
}
