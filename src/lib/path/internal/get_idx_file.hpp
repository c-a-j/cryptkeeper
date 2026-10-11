#include <filesystem>

namespace ck::index { 
  namespace fs = std::filesystem;
  fs::path get_idx_file(const std::string&);
}
