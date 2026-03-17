#pragma once
#include <gpgme.h>
#include <string>
#include <filesystem>

#include "lib/crypto/secure_bytes.hpp"

namespace ck::crypto {
  // std::string get_fingerprint();
  bool public_key_exists(const std::string&);
  bool private_key_exists(const std::string&);
  void init_gpgme();
  SecureBytes read_file(const std::filesystem::path&);
  SecureBytes decrypt_bytes(const SecureBytes&);
  SecureBytes encrypt_bytes(const SecureBytes&, const std::vector<std::string>&);
  SecureBytes encrypt_bytes(const SecureBytes&, const std::string&);
  void write_file(const SecureBytes&, const std::vector<std::string>&, const std::filesystem::path&);
  
  template<typename F>
  auto with_decrypted_bytes(const std::filesystem::path& path, F&& fn)
    -> decltype(fn(std::declval<const SecureBytes&>())) {
      const SecureBytes ciphertext = read_file(path);
      const SecureBytes plain = decrypt_bytes(ciphertext);
      return std::forward<F>(fn)(plain);
  }
  
  template<typename F>
  auto with_decrypted_bytes(const SecureBytes& ciphertext, F&& fn)
    -> decltype(fn(std::declval<const SecureBytes&>())) {
      const SecureBytes plain = decrypt_bytes(ciphertext);
      return std::forward<F>(fn)(plain);
  }
}
