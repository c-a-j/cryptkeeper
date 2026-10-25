#pragma once
#include <filesystem>
#include <gpgme.h>
#include <string>

#include "lib/crypto/secure_bytes.hpp"

namespace ck::crypto {
// std::string get_fingerprint();
bool public_key_exists(const std::string &key_fpr);
bool private_key_exists(const std::string &key_fpr);
void init_gpgme();
SecureBytes read_file(const std::filesystem::path &path);
SecureBytes decrypt_bytes(const SecureBytes &bytes);
SecureBytes encrypt_bytes(
    const SecureBytes &bytes,
    const std::vector<std::string> &key_fprs
);
SecureBytes encrypt_bytes(const SecureBytes &bytes, const std::string &key_fpr);
void write(
    const SecureBytes &bytes,
    const std::vector<std::string> &key_fprs,
    const std::filesystem::path &path
);

template <typename F>
auto with_decrypted_bytes(const std::filesystem::path &path, F &&fn)
    -> decltype(fn(std::declval<const SecureBytes &>())) {
    const SecureBytes ciphertext = read_file(path);
    const SecureBytes plain = decrypt_bytes(ciphertext);
    return std::forward<F>(fn)(plain);
}

template <typename F>
auto with_decrypted_bytes(const SecureBytes &ciphertext, F &&fn)
    -> decltype(fn(std::declval<const SecureBytes &>())) {
    const SecureBytes plain = decrypt_bytes(ciphertext);
    return std::forward<F>(fn)(plain);
}
} // namespace ck::crypto
