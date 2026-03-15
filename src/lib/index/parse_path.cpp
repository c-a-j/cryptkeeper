#include <nlohmann/json.hpp>
#include <string>

#include "lib/types.hpp"

namespace ck::index { 
  using namespace ck::secret;
  
  std::optional<std::vector<std::string>> parse_path(const Secret& secret) {
    if (!secret.path) { return std::nullopt; }
    
    std::string part;
    std::vector<std::string> path_parts;
    std::stringstream ss(*secret.path);
    
  
    while (std::getline(ss, part, '/')) {
      if (part == "" || part.empty()) { continue; }
      path_parts.push_back(part);
    }
    
    return(path_parts);
  };
}