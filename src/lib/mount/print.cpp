#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <optional>

#include "global.hpp"
#include "lib/mount/types.hpp"

#include "../path/path.hpp"
#include "../path/parse_path.hpp"
#include "./_internal/theme.hpp"

namespace fs = std::filesystem;

namespace {
  using ck::util::term::reset;
  using enum ck::mount::OutputComponent;

  inline const std::string_view ARROW = " \u27F6  ";
  // inline const std::string_view ARROW = " \u2192 "

  struct Node {
    std::optional<ck::mount::Mount> mount;
    std::unordered_map<std::string, Node> children;
  };

  Node* walk_path(Node* node, const std::string& alias) {
    std::vector<std::string> path = ck::path::parse_path(alias);

    for (std::size_t i = 0; i < path.size(); ++i) {
      node = &node->children[path[i]];
    }
    return node;
  }

  Node mount_tree(const ck::mount::Mounts& mounts) {
    Node root; 
    root.mount = mounts.root();

    for (auto [alias, mount] : mounts.mounts()) {
      Node* node = walk_path(&root, alias);
      node->mount = std::move(mount);
    }

    return root;
  }

  void print_root(const std::string& path) {
    std::cout 
      << get_scheme_ansi(RootMountAlias)
      << APP_NAME 
      << reset()
      << get_scheme_ansi(RootMountArrow)
      << ARROW
      << reset()
      << get_scheme_ansi(RootMountPath)
      << "(" << path << ")" 
      << reset()
      << "\n";
  }

  void print_child(
      const std::string& prefix, 
      const bool is_last, 
      const std::string& alias, 
      const std::string& path
    ) {
    std::cout 
      << get_scheme_ansi(Line)
      << prefix 
      << reset()
      << get_scheme_ansi(Line)
      << (is_last ? "└── " : "├── " )
      << reset()
      << get_scheme_ansi(MountAlias)
      << alias
      << reset()
      << get_scheme_ansi(MountArrow)
      << ARROW
      << reset()
      << get_scheme_ansi(MountPath)
      << "("
      << path
      << ")"
      << reset()
      << "\n";
  }

  void print_child(
      const std::string& prefix, 
      const bool is_last, 
      const std::string& alias
    ) {
    std::cout 
      << get_scheme_ansi(Line)
      << prefix 
      << reset()
      << get_scheme_ansi(Line)
      << (is_last ? "└── " : "├── " )
      << reset()
      << get_scheme_ansi(MountSubDir)
      << alias
      << reset()
      << "\n";
  }

  void print_tree(const Node& node, const std::string& prefix) {
    std::vector<std::string> aliases;
    aliases.reserve(node.children.size());

    for (const auto& [alias, _] : node.children) {
      aliases.push_back(alias);
    }

    std::sort(aliases.begin(), aliases.end());

    for (std::size_t i = 0; i < aliases.size(); ++i) {
      const bool is_last = (i + 1 == aliases.size());
      const auto& alias = aliases[i];
      const Node& child = node.children.at(alias);

      if (child.mount) {
        print_child(prefix, is_last, alias, child.mount->path);
      } else {
        print_child(prefix, is_last, alias);
      }
        
      print_tree(child, prefix + (is_last ? "    " : "│   "));
    }
  }

  void print_tree(const Node& node) {
    print_root(node.mount->path);
    print_tree(node, "");
  }
}

namespace ck::mount {
  void Mounts::print() {
    fs::path mnt_file = ck::path::mount_file();
    if (!fs::exists(mnt_file)) { return; }
    this->deserialize();
    Node root = mount_tree(*this);
    print_tree(root);
  }
}
