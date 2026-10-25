#pragma once

#include <string_view>
#include <vector>

namespace ck::config {
std::vector<std::string> parse_key(std::string_view key);
}
