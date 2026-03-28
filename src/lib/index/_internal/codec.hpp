#include "toml++/toml.hpp"

#include <string>
#include <vector>

#include "lib/index/types.hpp"

namespace ck::index::codec { 
  namespace fs = std::filesystem;
  Node deserialize(std::string_view toml_text, const fs::path& file_path);
  toml::table serialize(const Node* root);
}

