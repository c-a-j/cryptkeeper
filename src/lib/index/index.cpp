#include "util/error.hpp"
#include "lib/index/types.hpp"
#include "lib/crypto/crypto.hpp"
#include "global.hpp"
#include "../path/get_idx_file.hpp"
#include "./_internal/codec.hpp"

namespace {
}

namespace ck::index { 
  using namespace ck::crypto;
  using enum ck::util::error::IndexErrc;

  Index::Index(const std::string& vault_path) { 
    this->path_ = fs::path(vault_path);
    this->file_ = this->path_ / INDEX_FILE;
    this->load(vault_path); 
  }

  Index::Index(const std::string& vault_path, const std::string& alias) { 
    this->load(vault_path); 
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
