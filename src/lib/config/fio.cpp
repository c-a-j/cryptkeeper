#include "toml++/toml.hpp"

#include <filesystem>
#include <sstream>

#include "util/error.hpp"
#include "lib/config/types.hpp"
#include "util/logger/logger.hpp"

#include "./_internal/codec.hpp"
#include "../fio/atomic_write.hpp"
#include "../path/path.hpp"
#include "../path/existence.hpp"

namespace ck::config {
  using ck::util::error::Error;
  using ck::util::error::ConfigErrc;
  using enum ck::util::error::ConfigErrc;
  using ck::util::logger::logger;

  void Config::load() {
    std::filesystem::path cfg_file = ck::path::config_file();
    if (!ck::path::file_exists(cfg_file)) {
      this->state_.core.home = ck::path::vault_root();
      return;
    }

    std::ifstream in(cfg_file, std::ios::binary);
    std::string text(
      (std::istreambuf_iterator<char>(in)),
      std::istreambuf_iterator<char>());

    State state = codec::deserialize(text);
    this->state_ = state;
  }

  void Config::save() {
    ck::path::create_config_dir();  

    State state = this->state_;

    toml::table tbl = codec::serialize(state);
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
