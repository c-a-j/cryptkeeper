#pragma once

#include <vector>

namespace ck::crypto {
  void secure_wipe(void* ptr, std::size_t len);
}