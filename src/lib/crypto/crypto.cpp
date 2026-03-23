#include "lib/crypto/crypto.hpp"
#include <gpgme.h>
#include <stdexcept>
#include <cstddef>
#include <filesystem>
#include <fstream>

#include "./lib/crypto/secure_wipe.hpp"
#include "./lib/crypto/secure_bytes.hpp"
#include "util/error.hpp"

namespace {
  using ck::util::error::Error;
  using ck::util::error::CryptoErrc;
  using enum ck::util::error::CryptoErrc;

  gpgme_ctx_t make_openpgp_ctx() {
    ck::crypto::init_gpgme();

    gpgme_ctx_t ctx = nullptr;
    gpgme_error_t err = gpgme_new(&ctx);
    if (err) {
      throw Error<CryptoErrc>{GpgmeNewFailed, gpgme_strerror(err)};
    }

    err = gpgme_set_protocol(ctx, GPGME_PROTOCOL_OpenPGP);
    if (err) {
      gpgme_release(ctx);
      throw Error<CryptoErrc>{GpgmeSetProtocolFailed, gpgme_strerror(err)};
    }

    return ctx;
  }

  
  bool key_exists(const std::string& key_id, const bool secret) {
    gpgme_ctx_t ctx = make_openpgp_ctx();
    gpgme_key_t key = nullptr;
    
    gpgme_error_t err = gpgme_get_key(ctx, key_id.c_str(), &key, secret ? 1 : 0);
    if (key) {
      gpgme_key_unref(key);
    }
    gpgme_release(ctx);

    // not found is a normal false condition
    if (gpgme_err_code(err) == GPG_ERR_EOF || gpgme_err_code(err) == GPG_ERR_NOT_FOUND) {
      return false;
    }

    // any other error should surface
    if (err) {
      throw Error<CryptoErrc>{GpgmeGetKeyFailed, gpgme_strerror(err)};
    }

    return true;
  }
} 

  
namespace ck::crypto {
  void init_gpgme() {
    if (!gpgme_check_version(nullptr)) {
      throw Error<CryptoErrc>{GpgmeFailed, "gpgme initialization failed"};
    }
  }
  bool public_key_exists(const std::string& key_fpr) {return key_exists(key_fpr, false);}
  bool private_key_exists(const std::string& key_fpr) {return key_exists(key_fpr, true);}
  
  SecureBytes decrypt_bytes(const SecureBytes& bytes) {
    gpgme_ctx_t ctx = make_openpgp_ctx();
    gpgme_data_t cipher = nullptr;
    gpgme_data_t plain = nullptr;
    
    gpgme_error_t err = gpgme_data_new_from_mem(&cipher, bytes.char_data(), bytes.size(), 0);
    if (err) {
      gpgme_release(ctx);
      throw std::runtime_error(gpgme_strerror(err));
    }
    
    err = gpgme_data_new(&plain);
    if (err) {
      gpgme_release(ctx);
      gpgme_data_release(cipher);
      throw std::runtime_error(gpgme_strerror(err));
    }
    
    err = gpgme_op_decrypt(ctx, cipher, plain);
    gpgme_data_release(cipher);
    if (err) {
      gpgme_release(ctx);
      gpgme_data_release(plain);
      throw std::runtime_error(gpgme_strerror(err));
    }
    
    size_t plain_len = 0;
    char* plain_buf = gpgme_data_release_and_get_mem(plain, &plain_len);
    if (!plain_buf) {
      gpgme_release(ctx);
      throw std::runtime_error("failed to extract decrypted buffer");
    }
    
    SecureBytes result;
    result.assign(plain_buf, plain_len);
    secure_wipe(plain_buf, plain_len);
    gpgme_free(plain_buf);
    gpgme_release(ctx);
    return result;
  };
  
  SecureBytes read_file(const std::filesystem::path& path) {
    std::ifstream file(path);
    if (!file) {
      throw Error<CryptoErrc>{FailedToOpenFile, std::string(path)};
    }
    file.seekg(0, std::ios::end);
    const auto size = static_cast<std::size_t>(file.tellg());
    file.seekg(0, std::ios::beg);
    
    SecureBytes bytes(size);
    file.read(bytes.char_data(), static_cast<std::streamsize>(size));
    
    if (!file) {
      throw Error<CryptoErrc>{FailedToReadFile, std::string(path)};
    }
    return bytes;
  };
  
  void release_keys(std::vector<gpgme_key_t>& key_fprs) {
    for (auto& fpr : key_fprs) {
      gpgme_key_unref(fpr);
    }
  }
  
  std::vector<gpgme_key_t> add_sentinel(std::vector<gpgme_key_t>& keys) {
    std::vector<gpgme_key_t> k;
    k.reserve(keys.size() + 1);
    for (auto& key : keys) {
      k.push_back(key);
    }
    k.push_back(nullptr);
    return k;
  }
  
  SecureBytes encrypt_bytes(const SecureBytes& bytes, const std::vector<std::string>& key_fprs) {
    gpgme_ctx_t ctx = make_openpgp_ctx();
    gpgme_data_t cipher = nullptr;
    gpgme_data_t plain = nullptr;
    std::vector<gpgme_key_t> recipients;
    
    for (const auto& fpr : key_fprs) {
      gpgme_key_t key = nullptr;
      gpgme_error_t err = gpgme_get_key(ctx, fpr.c_str(), &key, 0);
      if (err) {
        gpgme_release(ctx);
        release_keys(recipients);
        throw std::runtime_error(gpgme_strerror(err));
      }
      if (!key) {
        gpgme_release(ctx);
        release_keys(recipients);
        throw Error<CryptoErrc>{GpgKeyNotFound, fpr};
      }
      recipients.push_back(key);
    }
    
    gpgme_error_t err = gpgme_data_new_from_mem(&plain, bytes.char_data(), bytes.size(), 0);
    if (err) {
      gpgme_release(ctx);
      release_keys(recipients);
      throw std::runtime_error(gpgme_strerror(err));
    }
    
    err = gpgme_data_new(&cipher);
    if (err) {
      gpgme_release(ctx);
      gpgme_data_release(plain);
      release_keys(recipients);
      throw std::runtime_error(gpgme_strerror(err));
    }
    
    gpgme_encrypt_flags_t flags{};
    err = gpgme_op_encrypt(
      ctx, 
      add_sentinel(recipients).data(), 
      flags,
      plain, 
      cipher
    );
    gpgme_data_release(plain);
    release_keys(recipients);
    if (err) {
      gpgme_release(ctx);
      gpgme_data_release(cipher);
      throw std::runtime_error(gpgme_strerror(err));
    }
    
    size_t cipher_len = 0;
    char* cipher_buf = gpgme_data_release_and_get_mem(cipher, &cipher_len);
    if (!cipher_buf) {
      gpgme_release(ctx);
      throw std::runtime_error("failed to extract encrypted buffer");
    }
   
    SecureBytes result;
    result.assign(cipher_buf, cipher_len);
    secure_wipe(cipher_buf, cipher_len);
    gpgme_free(cipher_buf);
    gpgme_release(ctx);
    return result;
  };

  SecureBytes encrypt_bytes(const SecureBytes& bytes, const std::string& key_fpr) {
    std::vector<std::string> fprs;
    fprs.push_back(key_fpr);
    return encrypt_bytes(bytes, fprs);
  }
  
  void write(const SecureBytes& plain, const std::vector<std::string>& key_fprs, const std::filesystem::path& path) {
    std::ofstream file(path, std::ios::binary | std::ios::trunc);
    if (!file) {
      throw Error<CryptoErrc>{FailedToOpenFile, std::string(path)};
    }
    const SecureBytes cipher = encrypt_bytes(plain, key_fprs);
    
    file.write(cipher.char_data(), static_cast<std::streamsize>(cipher.size()));
    if (!file) {
      throw Error<CryptoErrc>{FailedToReadFile, std::string(path)};
    }
  };
  
}
