#include <filesystem>
#include <string>

#include "./internal/fingerprints.hpp"
#include "./internal/tree.hpp"
#include "lib/config/config.hpp"
#include "lib/crypto/crypto.hpp"
#include "lib/crypto/random.hpp"
#include "lib/index/error.hpp"
#include "lib/index/index.hpp"
#include "lib/input/wisper.hpp"
#include "lib/path/parse_path.hpp"
#include "ui/console/console.hpp"

namespace {
using ck::error::Error;
using ck::error::IndexErrc;
using ck::ui::console::console;
using enum ck::error::IndexErrc;

struct FlatEntry {
    std::vector<std::string> path;
    std::string uuid;
};

FlatEntry create_entry(const std::string &path) {
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
} // namespace

namespace ck::index {
using ck::config::cfg;
using namespace ck::config;
using namespace ck::crypto;
using ck::error::Error;
using ck::error::IndexErrc;
using enum ck::error::IndexErrc;
void Index::insert(const std::string &path, const bool pwgen) {
    FlatEntry flat = create_entry(path);

    std::vector<std::string> keys = get_fingerprints(this->path_);

    ck::index::Entry entry;
    entry.uuid = flat.uuid;
    Node *node = tree::break_trail(&this->root_, flat.path);

    auto wisper = [](this auto &&self) -> SecureBytes {
        SecureBytes listen1 = ck::input::wisper("Enter secret:");
        SecureBytes listen2 = ck::input::wisper("Confirm secret:");
        if (listen1 != listen2) {
            console.error("Inputs do not match", "try again");
            self();
        }
        return listen1;
    };

    const SecureBytes secret = pwgen
        ? ck::crypto::pwgen(cfg.pwgen)
        : wisper();

    fs::path secret_file = this->path_ / fs::path(flat.uuid);

    ck::crypto::write(secret, keys, secret_file);
    node->entry = std::move(entry);
    this->save();
}

void Index::insert_node(const Node &node, const std::string &path) {
    std::vector<std::string> path_parts = ck::path::parse_path(path);
    Node *parent = tree::get_parent(&this->root_, path);
    parent->children[path_parts[path_parts.size() - 1]] = node;
}
} // namespace ck::index
