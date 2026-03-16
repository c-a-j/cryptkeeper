#include "toml++/toml.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

#include "lib/types.hpp"
#include "./path.hpp"
#include "./serialize.hpp"
#include "util/error.hpp"
#include "util/logger/logger.hpp"

inline constexpr std::string_view GLOBAL_CONFIGS = "global";

namespace ck::config {
  namespace fs = std::filesystem;
  using ck::util::error::Error;
  using ck::util::error::ConfigErrc;
  using enum ck::util::error::ConfigErrc;
  using ck::util::logger::logger;
  

  void save_config(Config& cfg) {
    create_config_dir();
    toml::table cfg_toml = serialize(cfg);
    fs::path cfg_file = app_config_file();
    bool exists = fs::exists(cfg_file);
    std::ofstream out(cfg_file, std::ios::out | std::ios::trunc);
    out << cfg_toml << "\n";
    if (!out) {
      throw Error<ConfigErrc>{SaveConfigFailed, std::string(cfg_file)};
    }
    if (!exists) {
      logger.info("Created new config file", std::string(cfg_file));
    }
  }
}
