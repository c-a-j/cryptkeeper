#pragma once
#include <optional>
#include <string>

namespace ck::tests::util {
class ScopedGnupgHome {
  public:
    ScopedGnupgHome();
    ~ScopedGnupgHome();
    const std::string &path() const;

  private:
    std::string tmp_home_;
    std::optional<std::string> home_;
};

std::string generate_tmp_key();
} // namespace ck::tests::util