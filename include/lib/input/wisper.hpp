#pragma once

#include "lib/crypto/secure_bytes.hpp"

namespace ck::input {
  ck::crypto::SecureBytes wisper(std::string_view prompt);
}
