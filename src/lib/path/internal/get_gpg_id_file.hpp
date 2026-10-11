#include <filesystem>

namespace ck::path { 
  namespace fs = std::filesystem;
  fs::path get_gpg_id_file(const std::string&);
}
