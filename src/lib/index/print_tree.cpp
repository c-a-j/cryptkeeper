#include <algorithm>
#include <iostream>

#include "util/term.hpp" 

#include "./_internal/types.hpp"
#include "./_internal/theme.hpp" 
 
namespace {
  using ck::util::term::reset;
  using ck::index::OutputComponent;
  using enum OutputComponent;

  void print_tree(const ck::index::Node& node, const std::string& prefix) {
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
    print_tree(this->root_, "");
  }
}
