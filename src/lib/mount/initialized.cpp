#include "global.hpp"
#include <filesystem>

#include "lib/path/existence.hpp"

namespace ck::mount {
namespace fs = std::filesystem;

bool initialized(const fs::path &path) {
    fs::path gpg_id_file = path / fs::path(GPG_ID_FILE);
    return ck::path::exists(gpg_id_file);
}
} // namespace ck::mount
