#include <string>
#include <filesystem>

#include "cli/types.hpp"

#include "util/error.hpp"
#include "util/logger/logger.hpp"

#include "lib/config/types.hpp"
#include "../path/parse_path.hpp"
#include "lib/crypto/crypto.hpp"
#include "lib/crypto/random.hpp"
#include "lib/input/wisper.hpp"

#include "lib/index/types.hpp"
#include "./_internal/tree.hpp"
#include "./_internal/fingerprints.hpp"

  
namespace {
  using ck::util::logger::logger;
  using ck::util::error::Error;
  using ck::util::error::IndexErrc;
  using enum ck::util::error::IndexErrc;

  struct FlatEntry {
    std::vector<std::string> path;
    std::string uuid;
  };

  FlatEntry create_entry(const std::string& path) {
    FlatEntry entry;
    std::vector<std::string> path_parts = ck::path::parse_path(path);
    std::vector<std::string> gid;
    
    if (path_parts.empty()) {
      throw Error<IndexErrc>{NoPath};
    }
    
    entry.path = path_parts;
    entry.uuid = ck::crypto::uuid_v4();
    return entry;
  }
}

namespace ck::index { 
  using ck::config::cfg;
  using namespace ck::config;
  using namespace ck::crypto;
  using ck::util::error::Error;
  using ck::util::error::IndexErrc;
  using enum ck::util::error::IndexErrc;
  void Index::insert(const std::string& path, const bool pwgen) {
    FlatEntry flat = create_entry(path);

    std::vector<std::string> keys = get_fingerprints(this->path_);

    ck::index::Entry entry;
    entry.uuid = flat.uuid;
    Node* node = tree::break_trail(&this->root_, flat.path);
    
    auto wisper = [](this auto&& self) -> const SecureBytes {
      SecureBytes listen1 = ck::input::wisper("Enter secret:");   
      SecureBytes listen2 = ck::input::wisper("Confirm secret:");   
      if (listen1.data() != listen2.data()) {
        logger.error("Inputs do not match", "try again");
        self();
      }
      return listen1;   
    };

    const SecureBytes secret = (pwgen) 
        ? ck::crypto::pwgen()
        : wisper();
    
    fs::path secret_file = this->path_ / fs::path(flat.uuid);

    ck::crypto::write(
      secret, 
      keys,
      secret_file
    );
    node->entry = std::move(entry);
    this->save();
  }

  void Index::insert_node(const Node& node, const std::string& path) {
    std::vector<std::string> path_parts = ck::path::parse_path(path);
    Node* parent = tree::get_parent(&this->root_, path);
    parent->children[path_parts[path_parts.size()-1]] = node;
  }
}
