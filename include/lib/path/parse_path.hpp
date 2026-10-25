#pragma once
#include <optional>
#include <string>
#include <vector>

namespace ck::path {
std::optional<std::vector<std::string>>
parse_path(const std::optional<std::string> &);
std::vector<std::string> parse_path(const std::string &);
} // namespace ck::path
