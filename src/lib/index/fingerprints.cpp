#include <fstream>
#include <filesystem>
#include <vector>

#include "../path/get_gpg_id_file.hpp"
#include "util/error.hpp"

namespace ck::index {
  using ck::util::error::Error;
  using ck::util::error::IndexErrc;
  using enum ck::util::error::IndexErrc;
  namespace fs = std::filesystem;
  
  std::vector<std::string> get_fingerprints(const std::string& vault_path) {
    fs::path gpg_id_file = ck::path::get_gpg_id_file(vault_path);
    std::ifstream file(gpg_id_file);
    if (!file.is_open()) {
      throw Error<IndexErrc>{GpgIdFileNotFound, gpg_id_file.string()};
    }
    std::vector<std::string> fingerprints;
    std::string buf;
    while (std::getline(file, buf)) {
      fingerprints.push_back(buf);
    }
    return fingerprints;
  } 
}
