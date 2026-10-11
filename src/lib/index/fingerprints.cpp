#include <fstream>
#include <filesystem>
#include <vector>

#include "util/error.hpp"

#include "../path/internal/get_gpg_id_file.hpp"

namespace ck::index {
  using ck::util::error::Error;
  using ck::util::error::IndexErrc;
  using enum ck::util::error::IndexErrc;
  namespace fs = std::filesystem;
  
  std::vector<std::string> get_fingerprints(const std::string& crypt_path) {
    fs::path gpg_id_file = ck::path::get_gpg_id_file(crypt_path);
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
