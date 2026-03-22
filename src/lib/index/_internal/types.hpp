#pragma once

#include <filesystem>
#include <string>
#include <optional>
#include <unordered_map>

namespace ck::index {
  namespace fs = std::filesystem;
  struct Entry {
    std::string uuid;
  };
  
  struct Node {
    std::optional<Entry> entry;
    std::unordered_map<std::string, Node> children;
  };
  
  class Index {
    public:
      explicit Index(const std::string& path = {});

      const Node root() const;
      Node root();
      
      void print();
      void write(const std::string&);
      void find(const std::string&);
      void deserialize(const std::string&);
    
    private:
      Node root_;
  };
}
