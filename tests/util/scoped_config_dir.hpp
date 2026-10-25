#pragma once
#include <optional>
#include <string>

namespace ck::tests::util {
class ScopedConfigDir {
  public:
    ScopedConfigDir();
    ~ScopedConfigDir();

  private:
    std::string tmp_dir_;
    std::optional<std::string> dir_;
};
} // namespace ck::tests::util