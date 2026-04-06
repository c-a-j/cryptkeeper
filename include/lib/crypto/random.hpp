#pragma once
#include <string>

#include "lib/crypto/types.hpp"
#include "lib/crypto/secure_bytes.hpp"

namespace ck::crypto {
  std::string uuid_v4();
  SecureBytes pwgen(const PwSpec&);
}
