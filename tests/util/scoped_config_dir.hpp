#pragma once
#include <string>
#include <optional>

namespace ck::tests::util {
  class ScopedConfigDir {
    public:
      ScopedConfigDir(); 
      ~ScopedConfigDir();
      
    private:
      std::string tmp_dir_;
      std::optional<std::string> dir_;
  };
}