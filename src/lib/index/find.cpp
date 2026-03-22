#include <nlohmann/json.hpp>
#include <string>
#include <iostream>

#include "global.hpp"
#include "util/error.hpp"
#include "util/logger/logger.hpp"
#include "lib/config/types.hpp"
#include "lib/mount/types.hpp"

#include "../path/parse_path.hpp"
#include "./_internal/types.hpp"
#include "./_internal/theme.hpp"

namespace ck::index { 
  using namespace index;
  using namespace ck::config;
  using namespace ck::mount;
  using ck::util::logger::logger;
  using ck::util::error::Error;
  using ck::util::error::IndexErrc;
  using enum ck::util::error::IndexErrc;
  using enum ck::index::OutputComponent;
  using namespace ck::util::term;
  
  void find(const std::optional<std::string>& path){
    Config cfg;
    Mounts mnt;
    Index idx;

    if (!path) {
      std::cout << "PRINTING WHOLE TREE" << "\n";
      return;
    }

    ResolvedPath rp = mnt.resolve(*path);
    idx.deserialize(rp.vault_path);

    if (!path) {
      std::cout << "PRINTING TREE" << "\n";
      return;
    }
    
    // std::optional<std::vector<std::string>> path_parts = ck::path::parse_path(path);
    // if (!path) {
    //   std::cout 
    //     << get_scheme_ansi(VaultName) 
    //     << APP_NAME
    //     << reset()
    //     << "\n";
    //     idx.print();
    //   return;
    // }
  //   Node node = idx.root();
  //   for (std::size_t i = 0; i < (*path).size(); ++i) {
  //     node = &node->children[(*path)[i]];
  //   }
  //   if (node->children.empty() && !node->entry) {
  //     throw Error<IndexErrc>{SecretNotFound, args.path.value_or("")};
  //   }
  //   std::cout 
  //     << get_scheme_ansi(NodeName) 
  //     << path->back()
  //     << reset()
  //     << "\n";
  //   print_tree(*node);
  }
}
