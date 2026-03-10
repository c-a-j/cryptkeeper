#include <nlohmann/json.hpp>
#include <string>
#include <iostream>

#include "util/logger.hpp"
#include "lib/types.hpp"
#include "lib/secret/secret.hpp"

namespace ck::lib::secret { 
  using namespace ck::types;
  using namespace ck::util::logger;

  std::vector<std::string> parse_path(const Secret& secret) {
    std::vector<std::string> path_parts;
    std::string part;
    std::stringstream ss(secret.path);
    
  
    while (std::getline(ss, part, '/')) {
      path_parts.push_back(part);
    }
    
    return(path_parts);
  };

  void serialize() {};
  
  void deserialize() {};
  
  void insert(const VaultConfig& cfg, const Secret& secret) {
    std::cout << "secret = " << secret.value << "\n";
    std::cout << "path = " << secret.path << "\n";
    std::vector<std::string> path_parts = parse_path(secret);

    for (const std::string& p : path_parts) {
      std::cout << p << "\n";
    }
  };
  
  
}
