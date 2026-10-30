#include <filesystem>
#include <fstream>
#include <vector>

#include "../path/internal/get_gpg_id_file.hpp"
#include "lib/index/error.hpp"

namespace ck::index {
using ck::error::Error;
using ck::error::IndexErrc;
using enum ck::error::IndexErrc;
namespace fs = std::filesystem;

std::vector<std::string> get_fingerprints(const std::string &crypt_path) {
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
} // namespace ck::index
