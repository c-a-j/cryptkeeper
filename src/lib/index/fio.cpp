#include "toml++/toml.hpp"
#include <sstream>
#include <filesystem>

#include "global.hpp"
#include "util/error.hpp"
#include "util/logger/logger.hpp"
#include "lib/index/types.hpp"
#include "lib/crypto/crypto.hpp"
#include "./_internal/vars.hpp"
#include "./_internal/codec.hpp"
#include "./_internal/types.hpp"
#include "../fio/atomic_write.hpp"
#include "../path/get_idx_file.hpp"
#include "./_internal/fingerprints.hpp"

namespace ck::index { 
  using namespace ck;
  using ck::util::logger::logger;
  using ck::util::error::Error;
  using ck::util::error::IndexErrc;
  using enum ck::util::error::IndexErrc;

  void Index::load(const std::string& vault_path, const std::string& alias) {
    fs::path path = fs::path(vault_path);
    fs::path file = path / INDEX_FILE;

    const crypto::SecureBytes cipher = crypto::read_file(file);
    const crypto::SecureBytes plain = crypto::decrypt_bytes(cipher);

    this->root_ = codec::deserialize(std::string_view(plain.char_data(), plain.size()), this->file_);
    this->path_ = path;
    this->file_ = file;
    this->alias_ = alias;
  }

  void Index::save() {
    toml::table tbl = codec::serialize(&this->root_);

    std::ostringstream out;
    out << tbl;
    std::string text = out.str();

    std::vector<std::string> fingerprints = get_fingerprints(this->path_);

    crypto::SecureBytes plain;
    plain.assign(text.data(), text.size());

    const crypto::SecureBytes cipher = crypto::encrypt_bytes(plain, fingerprints);
    
    fio::atomic_write(this->file_, cipher.bytes());
  }
}
