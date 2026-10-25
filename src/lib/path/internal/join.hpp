#pragma once
#include <string>
#include <vector>

namespace ck::path {
std::string join(const std::vector<std::string> &);
std::string join_suffix(const std::vector<std::string> &, std::size_t start);
std::string join_prefix(const std::vector<std::string> &, std::size_t start);
} // namespace ck::path
