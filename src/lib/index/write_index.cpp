#include <nlohmann/json.hpp>
#include <iostream>
#include <filesystem>
#include <fstream>

#include "util/error.hpp"
#include "lib/config/types.hpp"

#include "./_internal/get_idx_file.hpp"
#include "./_internal/deserialize.hpp"


namespace ck::index { 
  using namespace ck::config;
  using ck::util::error::Error;
  using ck::util::error::IndexErrc;
  using enum ck::util::error::IndexErrc;
  
  void write_idx(const VaultConfig& cfg, const nlohmann::json& j) {
    fs::path idx = get_idx_file(cfg);
    
    std::ofstream out(idx);
    if (!out) { throw std::runtime_error("failed to open index file"); }
    
    out << j.dump(2) << "\n";
  }
}