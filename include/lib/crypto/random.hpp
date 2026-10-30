#pragma once
#include <string>

#include "lib/crypto/secure_bytes.hpp"
#include "lib/config/pwgen.hpp"

namespace ck::crypto {
std::string uuid_v4();
SecureBytes pwgen(const ck::config::PwGen &cfg);
} // namespace ck::crypto
