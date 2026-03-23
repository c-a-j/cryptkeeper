#include <algorithm>
#include <iostream>

#include "global.hpp" 
#include "util/term.hpp" 
#include "util/error.hpp" 
#include "util/logger/logger.hpp" 
#include "lib/index/types.hpp"
#include "./_internal/theme.hpp" 
#include "../path/parse_path.hpp"
#include "../path/join.hpp"
 
namespace {
  using ck::util::logger::logger;
  using ck::util::error::Error;
  using ck::util::error::IndexErrc;
  using enum ck::util::error::IndexErrc;
  using ck::util::term::reset;
  using ck::index::OutputComponent;
  using enum OutputComponent;

  inline const std::string_view ARROW = " \u27F6  ";

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

  void print_top_node(const std::string& node_name) {
    std::cout 
      << get_scheme_ansi(NodeName)
      << node_name
      << reset()
      << "\n";
  }

  ck::index::Node find_node(ck::index::Node* root, const std::vector<std::string>& path_parts) {
    ck::index::Node* node = root;
    for (std::size_t i = 0; i < path_parts.size(); ++i) {
      node = &node->children[(path_parts[i])];
    }
    if (node->children.empty() && !node->entry) {
      logger.debug("Index::print() -> find_node()");
      throw Error<IndexErrc>{SecretNotFound, ck::path::join(path_parts)};
    }
    return *node;
  }

  void print_tree(const ck::index::Node& node, const std::string& prefix = "") {
    std::vector<std::string> names;
    names.reserve(node.children.size());
    
    for (const auto& [name, _] : node.children) {
      names.push_back(name);
    }
    
    std::sort(names.begin(), names.end());
    
    for (std::size_t i = 0; i < names.size(); ++i) {
      const bool is_last = (i + 1 == names.size());
      const auto& name = names[i];
      const ck::index::Node& child = node.children.at(name);
      
      std::cout 
        << get_scheme_ansi(Line)
        << prefix 
        << (is_last ? "└── " : "├── " )
        << reset()
        << (child.entry ? get_scheme_ansi(EntryName) : get_scheme_ansi(NodeName)) 
        << name 
        << reset() << "\n";
        
      print_tree(child, prefix + (is_last ? "    " : "│   "));
    }
  }
}

namespace ck::index {
  void Index::print() {
    print_root(this->path_);
    print_tree(this->root_);
  }

  void Index::print(const std::string& path) {
    std::vector<std::string> path_parts = ck::path::parse_path(path);
    Node node = find_node(&this->root_, path_parts);

    if (node.entry) {
      logger.debug("this is a terminal node");
    } else {
      print_top_node(path_parts[path_parts.size() - 1]);
    }
    print_tree(node);
  }
}
