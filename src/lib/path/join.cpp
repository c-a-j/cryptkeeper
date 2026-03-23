#include <string>
#include <vector>

namespace ck::path { 
  std::string join(const std::vector<std::string>& parts) {
    std::string out;
    bool first = true;
    for (auto part : parts) { 
      if (first) {
        out += part;
        first = false;
        continue;
      }
      out += "/" + part; 
    }
    return out;
  }

  std::string join_prefix(const std::vector<std::string>& parts, const std::size_t end) {
    std::string out;
    for (std::size_t i = 0; i < end; ++i) {
      if (i > 0) {
        out += "/";
      }
      out += parts[i]; 
    }
    return out;
  }

  std::string join_suffix(const std::vector<std::string>& parts, const std::size_t start) {
    std::string out;
    for (std::size_t i = start; i < parts.size(); ++i) {
      if (i > start) {
        out += "/";
      }
      out += parts[i]; 
    }
    return out;
  }
}
