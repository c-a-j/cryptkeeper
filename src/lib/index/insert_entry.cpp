#include <nlohmann/json.hpp>
#include <string>
#include <filesystem>

#include "cli/types.hpp"

#include "util/error.hpp"

#include "lib/config/types.hpp"
#include "lib/crypto/crypto.hpp"
#include "lib/crypto/random.hpp"
#include "lib/input/wisper.hpp"

#include "./_internal/types.hpp"
#include "./_internal/write_index.hpp"
#include "./_internal/parse_path.hpp"
#include "./_internal/walk_path.hpp"
#include "./_internal/serialize.hpp"
#include "./_internal/deserialize.hpp"
#include "./_internal/fingerprints.hpp"

namespace ck::index { 
  using namespace ck::config;
  using namespace ck::crypto;
  using ck::util::error::Error;
  using ck::util::error::IndexErrc;
  using enum ck::util::error::IndexErrc;
  
  IndexObj create_obj(
    const std::optional<std::string>& path
  ) {
    IndexObj obj;
    std::optional<std::vector<std::string>> path_components = parse_path(path);
    std::vector<std::string> gid;
    
    if (!path_components) {
      throw Error<IndexErrc>{NoPath};
    }
    
    obj.path = *path_components;
    obj.uuid = uuid_v4();
    return obj;
  }
  
  void insert_entry(
    Index& idx, 
    const IndexObj& obj, 
    const fs::path& file_path,
    std::vector<std::string>& keys,
    bool pwgen
  ) {
    Entry entry;
    entry.uuid = obj.uuid;
    std::vector<std::string> path_components = obj.path;
    Node* node = walk_path(&idx.root, path_components);
    
    const SecureBytes secret = (pwgen) 
        ? ck::crypto::pwgen() 
        : ck::input::wisper();
    
    ck::crypto::write_file(
      secret, 
      keys,
      file_path
    );
    node->entry = std::move(entry);
  }
  
  void insert(const VaultConfig& vcfg, const ck::cli::InsertArgs& args) {
    Index idx = deserialize(vcfg);
    IndexObj obj = create_obj(vcfg, args.path);
    
    fs::path sfp = secret_file_path(vcfg.directory, vcfg.vault, obj.uuid);
    std::vector<std::string> keys = get_fingerprints(vcfg);
    
    insert_entry(idx, obj, sfp, keys, args.pwgen);
    write_idx(vcfg, serialize(idx));
  }
}
