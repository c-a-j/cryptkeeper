#include <string>
#include <filesystem>

#include "cli/types.hpp"

#include "util/error.hpp"

#include "lib/config/types.hpp"
#include "../path/parse_path.hpp"
#include "lib/crypto/crypto.hpp"
#include "lib/crypto/random.hpp"
#include "lib/input/wisper.hpp"

#include "lib/index/types.hpp"
#include "./_internal/walk_path.hpp"
#include "./_internal/fingerprints.hpp"

  
namespace {
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
    Node* node = walk_path(&this->root_, flat.path);
    
    const SecureBytes secret = (pwgen) 
        ? ck::crypto::pwgen() 
        : ck::input::wisper();
    
    fs::path secret_file = this->path_ / fs::path(flat.uuid);

    ck::crypto::write(
      secret, 
      keys,
      secret_file
    );
    node->entry = std::move(entry);
    this->write();
  }
}
