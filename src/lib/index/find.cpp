#include <nlohmann/json.hpp>
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>

#include "lib/index/theme.hpp"
#include "lib/index/print_tree.hpp"
#include "lib/index/parse_path.hpp"
#include "lib/index/types.hpp"
#include "lib/index/find.hpp"
#include "lib/index/deserialize.hpp"
#include "lib/index/get_idx_file.hpp"

#include "util/logger/logger.hpp"
#include "util/error.hpp"
#include "lib/types.hpp"
#include "lib/crypto/crypto.hpp"
#include "lib/crypto/random.hpp"

namespace ck::index { 
  using namespace index;
  using namespace ck::config;
  using namespace ck::secret;
  using namespace ck::util::logger;
  using ck::util::error::Error;
  using ck::util::error::IndexErrc;
  using enum ck::util::error::IndexErrc;
  using enum ck::index::OutputComponent;
  using namespace ck::crypto;
  using namespace ck::util::term;
  
  void find(const VaultConfig& vcfg, const Secret& secret){
    Entry idx_entry;
    if (!vcfg.vault) {
      throw Error<IndexErrc>{VaultUnspecified, "vault must be specified as argument or in config file"};
    }
    Index idx = deserialize(vcfg);
    std::optional<std::vector<std::string>> path = parse_path(secret);
    if (!path) {
      std::cout 
        << get_scheme_ansi(VaultName) 
        << *vcfg.vault 
        << reset()
        << "\n";
        print_tree(idx.root);
      return;
    }
    Node* node = &idx.root;
    for (std::size_t i = 0; i < (*path).size(); ++i) {
      node = &node->children[(*path)[i]];
    }
    if (node->children.empty() && !node->entry) {
      throw Error<IndexErrc>{SecretNotFound, secret.path.value_or("")};
    }
    std::cout 
      << get_scheme_ansi(NodeName) 
      << path->back()
      << reset()
      << "\n";
    print_tree(*node);
  }
}