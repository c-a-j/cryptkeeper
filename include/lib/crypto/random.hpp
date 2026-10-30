#pragma once
#include <string>

#include "lib/config/pwgen.hpp"
#include "lib/crypto/secure_bytes.hpp"

namespace ck::crypto {
std::string uuid_v4();
SecureBytes pwgen(const ck::config::PwgenCfg &cfg);
} // namespace ck::crypto
