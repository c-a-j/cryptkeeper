#pragma once
#include <string>

namespace ck::crypto {
  void throw_gpgme_error(const std::string& msg, const std::string& gpgme_err);
  std::string gen_key();
  bool key_exists(const std::string&, bool);
  bool public_key_exists(std::string&);
  bool private_key_exists(std::string&);
  void init_gpgme();
}
