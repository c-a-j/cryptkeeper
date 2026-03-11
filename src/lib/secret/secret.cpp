#include <nlohmann/json.hpp>
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>

#include "util/logger.hpp"
#include "lib/types.hpp"
#include "lib/crypto/crypto.hpp"
#include "lib/crypto/random.hpp"
#include "lib/secret/secret.hpp"

namespace ck::secret { 
  using namespace index;
  using namespace ck::config;
  using namespace ck::util::logger;
  using namespace ck::crypto;
  namespace fs = std::filesystem;

  std::vector<std::string> parse_path(const Secret& secret) {
    std::vector<std::string> path_parts;
    std::string part;
    std::stringstream ss(secret.path);
    
  
    while (std::getline(ss, part, '/')) {
      if (part == "" || part.empty()) { continue; }
      path_parts.push_back(part);
    }
    
    return(path_parts);
  };
  
  fs::path get_idx_file(const VaultConfig& cfg) {
    fs::path idx = fs::path(*cfg.directory) / fs::path(*cfg.vault) / "idx.json";
    return(idx);
  }
  
  void create_index_file(const VaultConfig& cfg) {
    fs::path idx = get_idx_file(cfg);
    if (fs::exists(idx)) { return; }
    
    std::ofstream out(idx, std::ios::out | std::ios::trunc);
    out << "hello" << "\n";
  }

  void serialize() {};
  
  void deserialize() {};
  
  IndexEntry create_entry(const Secret& secret) {
    IndexEntry entry;
    entry.path = parse_path(secret);
    
    return entry;
  }
  
  void insert(const VaultConfig& cfg, const Secret& secret) {
    std::cout << "secret = " << secret.value << "\n";
    std::cout << "path = " << secret.path << "\n";
    std::vector<std::string> path_parts = parse_path(secret);

    for (const std::string& p : path_parts) {
      std::cout << p << "\n";
    }
    create_index_file(cfg);
    std::string foo = uuid_v4();
    std::cout << foo << "\n";
  };
  
  
}
