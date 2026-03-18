#include <nlohmann/json.hpp>
#include <string>
#include <iostream>

#include "cli/types.hpp"
#include "util/error.hpp"
#include "util/logger/logger.hpp"
#include "lib/types.hpp"

#include "lib/index/find.hpp"

#include "./_internal/types.hpp"
#include "./_internal/deserialize.hpp"
#include "./_internal/get_idx_file.hpp"
#include "./_internal/parse_path.hpp"
#include "./_internal/print_tree.hpp"
#include "./_internal/theme.hpp"

namespace ck::index { 
  using namespace index;
  using namespace ck::config;
  using ck::util::logger::logger;
  using ck::util::error::Error;
  using ck::util::error::IndexErrc;
  using enum ck::util::error::IndexErrc;
  using enum ck::index::OutputComponent;
  using namespace ck::crypto;
  using namespace ck::util::term;
  
  void find(const VaultConfig& vcfg, const ck::cli::ShowArgs& args){
    Entry idx_entry;
    if (!vcfg.vault) {
      throw Error<IndexErrc>{VaultUnspecified, "vault must be specified as argument or in config file"};
    }
    Index idx = deserialize(vcfg);
    std::optional<std::vector<std::string>> path = parse_path(args.path);
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
      throw Error<IndexErrc>{SecretNotFound, args.path.value_or("")};
    }
    std::cout 
      << get_scheme_ansi(NodeName) 
      << path->back()
      << reset()
      << "\n";
    print_tree(*node);
  }
}