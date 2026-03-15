#include <algorithm>
#include <iostream>

#include "lib/index/types.hpp" 
#include "lib/index/theme.hpp" 
#include "lib/index/print_tree.hpp" 
#include "util/term.hpp" 
 
namespace ck::index {
  using ck::util::term::reset;
  using enum OutputComponent;
  
  void print_tree(const Node& node, const std::string& prefix) {
    std::vector<std::string> names;
    names.reserve(node.children.size());
    
    for (const auto& [name, _] : node.children) {
      names.push_back(name);
    }
    
    std::sort(names.begin(), names.end());
    
    for (std::size_t i = 0; i < names.size(); ++i) {
      const bool is_last = (i + 1 == names.size());
      const auto& name = names[i];
      const Node& child = node.children.at(name);
      
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