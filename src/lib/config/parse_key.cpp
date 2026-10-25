#include <sstream>
#include <string>
#include <vector>

#include "internal/parse_key.hpp"

namespace ck::config {
std::vector<std::string> parse_key(std::string_view key) {
    std::string part;
    std::stringstream ss(std::string{key});
    std::vector<std::string> key_parts;
    key_parts.reserve(2);

    while (std::getline(ss, part, '.')) {
        key_parts.push_back(part);
    }
    return key_parts;
}
} // namespace ck::config
