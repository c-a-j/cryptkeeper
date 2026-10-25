#include <filesystem>

#include "global.hpp"

namespace ck::index {
namespace fs = std::filesystem;

// fs::path secret_file_path(
//   std::optional<std::string> root_dir,
//   std::optional<std::string> crypt_name,
//   std::string uuid
// ) {
//   if (!root_dir || !crypt_name) {
//     throw Error<IndexErrc>{UndefinedOptional, "crypt directory or crypt name
//     unspecified"};
//   }
//   fs::path sfp = fs::path(*root_dir) / fs::path(*crypt_name) / fs::path(uuid
//   + ".gpg"); return sfp;
// }

fs::path get_secret_file(const std::string &path) {
    fs::path idx_file = fs::path(path) / INDEX_FILE;
    return idx_file;
}
} // namespace ck::index
