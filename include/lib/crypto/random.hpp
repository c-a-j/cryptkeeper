#pragma once
#include <string>

#include "lib/crypto/secure_bytes.hpp"
#include "lib/crypto/types.hpp"

namespace ck::crypto {
std::string uuid_v4();
SecureBytes pwgen(const PwSpec &);
} // namespace ck::crypto
