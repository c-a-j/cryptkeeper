#pragma once

#include <cstddef>

namespace ck::fs {
  int write_all(int fd, const char*, std::size_t);
}
