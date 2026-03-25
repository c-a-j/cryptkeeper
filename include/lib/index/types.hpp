#pragma once

#include <filesystem>
#include <string>
#include <vector>
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
    std::optional<std::string> path; // vault path, only defined at root node
  };
  
  class Index {
    public:
      explicit Index(const std::string&);
      explicit Index(const std::string&, const std::string&);
      explicit Index() = default;

      const Node root() const;
      Node root();
      
      static Index empty(const std::string&);
      void deserialize(const std::string&);
      void deserialize(const std::string&, const std::string&);
      void insert(const std::string&, const bool);
      void show(const std::optional<std::string>&);
      void write();
      void insert_node(const Node&, const std::string&);
      void print(const bool = true);
      void print(const std::string&);
      bool secret_along_path(const std::vector<std::string>&);
      bool secret_along_path(const std::string&);
    
    private:
      Node root_;
      std::string alias_; // mount alias
      fs::path path_; // vault path
      fs::path file_; // full path to index file
      
      Node* break_trail(const std::vector<std::string>&);
      Node* break_trail(const std::string&);

      Node* walk_path(const std::vector<std::string>&);
      Node* walk_path(const std::string&);

      Node* get_parent(const std::vector<std::string>&);
      Node* get_parent(const std::string&);
  };
}
