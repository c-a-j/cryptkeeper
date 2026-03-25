#include "util/error.hpp"
#include "lib/index/types.hpp"
#include "global.hpp"
#include "../path/get_idx_file.hpp"

namespace {
}

namespace ck::index { 
  using enum ck::util::error::IndexErrc;

  Index::Index(const std::string& vault_path) { 
    this->deserialize(vault_path); 
  }

  Index::Index(const std::string& alias, const std::string& vault_path) { 
    this->deserialize(vault_path); 
    this->alias_ = alias;
  }

  Index Index::empty(const std::string& vault_path) {
    Index idx;
    idx.path_ = fs::path(vault_path);
    idx.file_ = idx.path_ / fs::path(INDEX_FILE);
    idx.root_ = Node{};
    idx.root_.path = vault_path;
    return idx;
  }

  Node Index::root() {
    return this->root_;
  }

  const Node Index::root() const {
    return this->root_;
  }
}
