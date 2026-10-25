#include "lib/index/index.hpp"
#include "global.hpp"
#include "lib/crypto/crypto.hpp"
#include "lib/index/error.hpp"

#include "../path/internal/get_idx_file.hpp"
#include "./internal/codec.hpp"

namespace {}

namespace ck::index {
using namespace ck::crypto;
using enum ck::error::IndexErrc;

Index::Index(const std::string &crypt_path) {
    this->path_ = fs::path(crypt_path);
    this->file_ = this->path_ / INDEX_FILE;
    this->load(crypt_path);
}

Index::Index(const std::string &crypt_path, const std::string &alias) {
    this->load(crypt_path);
    this->alias_ = alias;
}

Index Index::empty(const std::string &crypt_path) {
    Index idx;
    idx.path_ = fs::path(crypt_path);
    idx.file_ = idx.path_ / fs::path(INDEX_FILE);
    idx.root_ = Node{};
    idx.root_.path = crypt_path;
    return idx;
}

Node &Index::root() {
    return this->root_;
}

const Node &Index::root() const {
    return this->root_;
}
} // namespace ck::index
