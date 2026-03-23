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
      explicit Index(const std::string&);
      explicit Index();

      const Node root() const;
      Node root();
      
      void deserialize(const std::string&);
      void insert(const std::string&, const bool);
      void find(const std::string&);
      void write();
      void print();
    
    private:
      Node root_;
      fs::path path_; // vault path
      fs::path file_; // full path to index file
  };
}
