#include "toml++/toml.hpp"
#include <filesystem>
#include <sstream>

#include "global.hpp"
#include "lib/crypto/crypto.hpp"
#include "lib/crypto/error.hpp"
#include "lib/fio/atomic_write.hpp"
#include "lib/index/error.hpp"
#include "lib/index/index.hpp"
#include "ui/console/console.hpp"

#include "../path/internal/get_idx_file.hpp"
#include "./internal/codec.hpp"
#include "./internal/fingerprints.hpp"
#include "./internal/types.hpp"

namespace ck::index {
using namespace ck;
using ck::error::Error;
using ck::error::IndexErrc;
using ck::ui::console::console;
using enum ck::error::IndexErrc;

void Index::load(const std::string &crypt_path, const std::string &alias) {
    fs::path path = fs::path(crypt_path);
    fs::path file = path / INDEX_FILE;

    const crypto::SecureBytes cipher = crypto::read_file(file);
    const crypto::SecureBytes plain = crypto::decrypt_bytes(cipher);

    this->root_ = codec::deserialize(
        std::string_view(plain.char_data(), plain.size()), path
    );
    this->path_ = path;
    this->root_.path = path;
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

    const crypto::SecureBytes cipher =
        crypto::encrypt_bytes(plain, fingerprints);

    fio::atomic_write(this->file_, cipher.bytes());
}
} // namespace ck::index
