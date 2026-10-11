#pragma once

#include <cstddef>

namespace ck::fio {
  int write_all(int fd, const char*, std::size_t);
}
