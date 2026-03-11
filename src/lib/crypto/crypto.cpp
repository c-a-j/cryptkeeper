#include "lib/crypto.hpp"
#include <gpgme.h>
#include <stdexcept>

namespace ck::crypto {
  void throw_gpgme_error(const std::string& msg, const std::string& gpgme_err) {
    throw std::runtime_error(msg + std::string(": ") + gpgme_err);
  }
  
  void init_gpgme() {
    if (!gpgme_check_version(nullptr)) {
      throw std::runtime_error("gpgme initialization failed");
    }
  }
  
  bool key_exists(const std::string& key_id, bool secret) {
    init_gpgme();

    gpgme_ctx_t ctx = nullptr;
    gpgme_key_t key = nullptr;

    gpgme_error_t err = gpgme_new(&ctx);
    if (err) {
      throw_gpgme_error("gpgme_new_failed", gpgme_strerror(err));
    }

    err = gpgme_get_key(ctx, key_id.c_str(), &key, secret ? 1 : 0);

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
      throw_gpgme_error("gpgme_get_key failed", gpgme_strerror(err));
    }

    return true;
  }
  
  bool public_key_exists(std::string& key_fpr) {return key_exists(key_fpr, false);}
  bool private_key_exists(std::string& key_fpr) {return key_exists(key_fpr, true);}
}
