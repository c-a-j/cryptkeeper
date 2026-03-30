#pragma once

#include <cstddef>

namespace ck::crypto {
  struct PwSpec {
    std::size_t length;
    std::size_t symbols;
    std::size_t numbers;
    std::size_t uppercase;
    std::size_t lowercase;
  };
}
