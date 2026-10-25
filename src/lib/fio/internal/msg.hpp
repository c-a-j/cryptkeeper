#pragma once

#include <filesystem>
#include <string>

namespace ck::fio {
namespace fs = std::filesystem;
std::string msg(const fs::path &, int);
} // namespace ck::fio
