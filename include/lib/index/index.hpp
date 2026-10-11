#pragma once

#include <filesystem>
#include <string>
#include <optional>
#include <unordered_map>

namespace ck::index {
  namespace fs = std::filesystem;
  // entry in index tree
  struct Entry {
    std::string uuid;
  };
  
  struct Node {
    std::optional<Entry> entry;
    std::unordered_map<std::string, Node> children;
    std::optional<std::string> path; // crypt path, only defined at root node
  };

  struct IndexTree {
    Node root;
  };
  
  class Index {
    public:
      explicit Index(const std::string& crypt_path);
      explicit Index(const std::string& crypt_path, const std::string& alias);
      explicit Index() = default;

      const Node& root() const;
      Node& root();
      
      static Index empty(const std::string& crypt_path);
      void insert(const std::string& path, bool pwgen);
      void show(const std::optional<std::string>&);
      void insert_node(const Node& n, const std::string& path);
      void print(bool is_root = true);
      void print(const std::string& path);

      void load(const std::string& crypt_path, const std::string& alias = {});
      void save();
    
    private:
      Node root_;
      std::string alias_; // mount alias
      fs::path path_; // crypt path
      fs::path file_; // full path to index file
  };
}
