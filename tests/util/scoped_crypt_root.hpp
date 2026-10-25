#pragma once
#include <optional>
#include <string>

namespace ck::tests::util {
class ScopedCryptRoot {
  public:
    ScopedCryptRoot();
    ~ScopedCryptRoot();

  private:
    std::string tmp_root_;
    std::optional<std::string> root_;
};
} // namespace ck::tests::util