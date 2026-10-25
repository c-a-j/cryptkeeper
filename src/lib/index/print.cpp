#include <algorithm>
#include <iostream>

#include "global.hpp"
#include "lib/crypto/crypto.hpp"
#include "lib/index/error.hpp"
#include "lib/index/index.hpp"
#include "lib/path/parse_path.hpp"
#include "ui/console/console.hpp"
#include "ui/term.hpp"

#include "../path/internal/join.hpp"
#include "./internal/theme.hpp"

namespace {
using ck::error::Error;
using ck::error::IndexErrc;
using ck::ui::console::console;
using enum ck::error::IndexErrc;
using ck::index::OutputComponent;
using ck::ui::term::reset;
using enum OutputComponent;

inline const std::string_view ARROW = " \u27F6  ";

void print_root(const std::string &path) {
    std::cout << get_scheme_ansi(RootMountAlias) << APP_NAME << reset()
              << get_scheme_ansi(RootMountArrow) << ARROW << reset()
              << get_scheme_ansi(RootMountPath) << "(" << path << ")" << reset()
              << "\n";
}

void print_mount(const std::string &alias, const std::string &path) {
    std::vector<std::string> alias_parts = ck::path::parse_path(alias);
    std::cout << get_scheme_ansi(MountAlias)
              << alias_parts[alias_parts.size() - 1] << reset()
              << get_scheme_ansi(MountArrow) << ARROW << reset()
              << get_scheme_ansi(MountPath) << "(" << path << ")" << reset();
}

void print_top_node(const std::string &node_name) {
    std::cout << get_scheme_ansi(NodeName) << node_name << reset() << "\n";
}

ck::index::Node
find_node(ck::index::Node *root, const std::vector<std::string> &path_parts) {
    ck::index::Node *node = root;

    for (const auto &part : path_parts) {
        node = &node->children[part];
    }

    if (node->children.empty() && !node->entry) {
        console.debug("Index::print() -> find_node()");
        throw Error<IndexErrc>{SecretNotFound, ck::path::join(path_parts)};
    }

    return *node;
}

void print_tree(const ck::index::Node &node, const std::string &prefix = "") {
    std::vector<std::string> names;
    names.reserve(node.children.size());

    for (const auto &[name, _] : node.children) {
        names.push_back(name);
    }

    std::ranges::sort(names);

    for (std::size_t i = 0; i < names.size(); ++i) {
        const bool is_last = (i + 1 == names.size());
        const auto &name = names[i];
        const ck::index::Node &child = node.children.at(name);

        std::cout << get_scheme_ansi(Line) << prefix
                  << (is_last ? "└── " : "├── ") << reset();

        if (child.entry) {
            std::cout << get_scheme_ansi(EntryName) << name << reset();
        } else if (!child.entry && !child.path) {
            std::cout << get_scheme_ansi(NodeName) << name << reset();
        } else if (child.path) {
            print_mount(name, *child.path);
        }
        std::cout << "\n";

        print_tree(child, prefix + (is_last ? "    " : "│   "));
    }
}
} // namespace

namespace ck::index {
void Index::print(const bool is_root) {
    if (is_root) {
        print_root(this->path_);
    } else {
        print_mount(this->alias_, this->path_);
        std::cout << "\n";
    }
    print_tree(this->root_);
}

void Index::print(const std::string &path) {
    std::vector<std::string> path_parts = ck::path::parse_path(path);
    Node node = find_node(&this->root_, path_parts);

    if (node.entry) {
        fs::path sec_file = this->path_ / node.entry->uuid;
        ck::crypto::SecureBytes cipher = ck::crypto::read_file(sec_file);
        ck::crypto::SecureBytes plain = ck::crypto::decrypt_bytes(cipher);
        std::cout.write(
            plain.char_data(), static_cast<std::streamsize>(plain.size())
        );
        std::cout << "\n";
        return;
    }

    print_top_node(path_parts[path_parts.size() - 1]);
    print_tree(node);
}
} // namespace ck::index
