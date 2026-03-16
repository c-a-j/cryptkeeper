#pragma once
#include <string>

#include "lib/types.hpp"

namespace ck::crypto {
  std::string fingerprint(const ck::config::Vault& = {}, const ck::secret::Secret& = {});
  bool public_key_exists(std::string&);
  bool private_key_exists(std::string&);
  void init_gpgme();
}
