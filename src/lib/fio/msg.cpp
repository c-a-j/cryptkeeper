#include <filesystem>
#include <string>

namespace ck::fio {
namespace fs = std::filesystem;

std::string msg(const fs::path &path, int err) {
    return "[" + std::to_string(err) + "] " + path.string();
}
} // namespace ck::fio
