#include <nlohmann/json.hpp>
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>

#include "util/logger.hpp"
#include "util/term.hpp"
#include "util/error.hpp"
#include "lib/types.hpp"
#include "lib/crypto/crypto.hpp"
#include "lib/crypto/random.hpp"
#include "lib/index/index.hpp"

namespace ck::index { 
  using namespace index;
  using namespace ck::config;
  using namespace ck::util::logger;
  using ck::util::error::Error;
  using ck::util::error::IndexErrc;
  using enum ck::util::error::IndexErrc;
  using namespace ck::crypto;
  using namespace ck::util::term;
  namespace fs = std::filesystem;

  std::optional<std::vector<std::string>> parse_path(const Secret& secret) {
    if (!secret.path) { return std::nullopt; }
    
    std::string part;
    std::vector<std::string> path_parts;
    std::stringstream ss(*secret.path);
    
  
    while (std::getline(ss, part, '/')) {
      if (part == "" || part.empty()) { continue; }
      path_parts.push_back(part);
    }
    
    return(path_parts);
  };
  
  fs::path get_idx_file(const VaultConfig& cfg) {
    fs::path idx = fs::path(*cfg.directory) / fs::path(*cfg.vault) / "idx";
    return(idx);
  }
  
  nlohmann::json serialize_obj(const IndexObj& entry) {
    nlohmann::json j = {
      {"path", entry.path},
      {"uuid", entry.uuid},
    };
    if (entry.key_fpr) {
      j["key_fpr"] = *entry.key_fpr;
    }
    return j;
  }
  
  IndexObj deserialize_obj(const nlohmann::json& j) {
    IndexObj obj;
    obj.path = j["path"];
    obj.uuid = j["uuid"];
    if (j.contains("key_fpr")) obj.key_fpr = j["key_fpr"];
    return obj;
  }
  
  Entry deserialize_entry(const nlohmann::json& j) {
    Entry obj;
    obj.uuid = j["uuid"];
    if (j.contains("key_fpr")) obj.key_fpr = j["key_fpr"];
    return obj;
  }
  
  nlohmann::json deserialize_idx(const VaultConfig& cfg) {
    fs::path idx = get_idx_file(cfg);
    nlohmann::json j = nlohmann::json::array();
    if (!fs::exists(idx)) { return j; }
    std::ifstream in(idx);
    if (!in) { throw std::runtime_error("failed to open index file"); }
    in >> j;
    return j;
  };
  
  IndexObj create_obj(const Secret& secret) {
    IndexObj obj;
    std::optional<std::vector<std::string>> path = parse_path(secret);
    if (!path) {
      throw Error<IndexErrc>{NoPath};
    }
    obj.path = *path;
    obj.uuid = uuid_v4();
    return obj;
  }
  
  void write_idx(const VaultConfig& cfg, const nlohmann::json& j) {
    fs::path idx = get_idx_file(cfg);
    
    std::ofstream out(idx);
    if (!out) { throw std::runtime_error("failed to open index file"); }
    
    out << j.dump(2) << "\n";
  }
  
  void walk(Node* node, Entry& entry, const std::vector<std::string> path) {
    for (std::size_t i = 0; i < path.size(); ++i) {
      const bool is_last = (i + 1 == path.size());
      
      if (node->entry && !is_last){
        throw std::runtime_error("entry already exists in the path");
      }
      
      node = &node->children[path[i]];
    }
    
    if (node->entry) {
      throw std::runtime_error("duplicate path");
    }
    
    if (!node->children.empty()) {
      throw std::runtime_error("path conflict");
    }
    
    node->entry = std::move(entry);
  }
  
  Index load_index(const nlohmann::json& j) {
    Index idx;
    
    for (auto& obj : j) {
      Entry entry = deserialize_entry(obj);
      std::vector<std::string> path = obj["path"];
      
      Node* node = &idx.root;
      walk(node, entry, path);
    }
    
    return idx;
  }
  
  void insert_entry(Index& idx, const IndexObj& obj) {
    Entry entry;
    entry.uuid = obj.uuid;
    entry.key_fpr = obj.key_fpr;
    std::vector<std::string> path = obj.path;
    Node* node = &idx.root;
    walk(node, entry, path);
  }
  
  void dfs(const Node& node, std::vector<std::string>& path, nlohmann::json& out) {
    if (node.entry) {
      IndexObj obj;
      obj.path = path;
      obj.uuid = node.entry->uuid;
      obj.key_fpr = node.entry->key_fpr;
      out.push_back(serialize_obj(obj));
    }
    
    for (const auto& [key, child] : node.children) {
      path.push_back(key);
      dfs(child, path, out);
      path.pop_back();
    }
  }
  nlohmann::json serialize_index(const Index& idx) {
    nlohmann::json j = nlohmann::json::array();
    std::vector<std::string> path;
    dfs(idx.root, path, j);
    
    return j;
  }
  
  void insert(const VaultConfig& vcfg, const Secret& secret) {
    nlohmann::json j = deserialize_idx(vcfg);
    Index idx = load_index(j);
    IndexObj obj = create_obj(secret);
    insert_entry(idx, obj);
    j = serialize_index(idx);
    write_idx(vcfg, j);
  };
  
  enum class PrintComponent {
    VaultName,
    NodeName,
    EntryName,
    Line
  };
  
  Color get_scheme_color(PrintComponent c) {
    switch (c) {
      case PrintComponent::VaultName:       return Color::Purple;
      case PrintComponent::NodeName:        return Color::Blue;
      case PrintComponent::EntryName:       return Color::Yellow;
      case PrintComponent::Line:            return Color::Gray;
    }
    return Color::Gray;
  }
  
  std::string get_scheme_ansi(PrintComponent c) {
    return ansi(get_scheme_color(c));
  }
  
  using enum PrintComponent;
  
  void print_tree(const Node& node, const std::string& prefix = "") {
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
        << ansi(Color::Reset)
        << (child.entry ? get_scheme_ansi(EntryName) : get_scheme_ansi(NodeName)) 
        << name 
        << ansi(Color::Reset) << "\n";
        
      print_tree(child, prefix + (is_last ? "    " : "│   "));
    }
  }
  
  void find(const VaultConfig& vcfg, const Secret& secret){
    Entry idx_entry;
    if (!vcfg.vault) {
      throw Error<IndexErrc>{VaultUnspecified, "vault must be specified as argument or in config file"};
    }
    nlohmann::json j = deserialize_idx(vcfg);
    Index idx = load_index(j);
    std::optional<std::vector<std::string>> path = parse_path(secret);
    if (!path) {
      std::cout 
        << get_scheme_ansi(VaultName) 
        << *vcfg.vault 
        << ansi(Color::Reset) 
        << "\n";
        print_tree(idx.root);
      return;
    }
    Node* node = &idx.root;
    for (std::size_t i = 0; i < (*path).size(); ++i) {
      node = &node->children[(*path)[i]];
    }
    if (node->children.empty() && !node->entry) {
      throw Error<IndexErrc>{SecretNotFound, secret.path.value_or("")};
    }
    std::cout 
      << get_scheme_ansi(NodeName) 
      << path->back()
      << ansi(Color::Reset) 
      << "\n";
    print_tree(*node);
  }
}