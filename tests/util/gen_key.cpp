#include <filesystem>
#include <stdexcept>
#include <gpgme.h>
#include <gtest/gtest.h>

#include "lib/crypto/crypto.hpp"
#include "gen_key.hpp"

using namespace ck::crypto;

namespace fs = std::filesystem;

namespace ck::tests::util {
  ScopedGnupgHome::ScopedGnupgHome() {
    char tmpl[] = "/tmp/ck-gnupg-XXXXXX";
    char* dir = mkdtemp(tmpl);
    if (dir == nullptr) {
      throw std::runtime_error("mkdtemp failed");
    }
    tmp_home_ = dir;
    
    // GnuPG requires restrictive permissions on home dir
    if (chmod(tmp_home_.c_str(), 0700) != 0) {
      throw std::runtime_error("chmod 0700 failed for GNUPGHOME");
    }

    if (const char* old = std::getenv("GNUPGHOME")) {
      home_ = old;
    }
    if (setenv("GNUPGHOME", tmp_home_.c_str(), 1) != 0) {
      throw std::runtime_error("setenv(GNUPGHOME) failed");
    }
  }

  ScopedGnupgHome::~ScopedGnupgHome() {
    if (home_.has_value()) {
      setenv("GNUPGHOME", home_->c_str(), 1);
    } else {
      unsetenv("GNUPGHOME");
    }
    std::error_code ec;
    fs::remove_all(tmp_home_, ec);
  }

  const std::string& ScopedGnupgHome::path() const { 
    return tmp_home_; 
  }
  
  std::string generated_fpr_;
  std::string tmp_home_ = "/tmp/gnupg";
  
  std::string generate_tmp_key() {
    init_gpgme();
    
    gpgme_ctx_t ctx = nullptr;
    gpgme_error_t err = gpgme_new(&ctx);
    if (err) {
      gpgme_release(ctx);
      throw_gpgme_error("gpgme_new failed", gpgme_strerror(err));
    }
    
    err = gpgme_set_protocol(ctx, GPGME_PROTOCOL_OpenPGP);
    if (err) {
      gpgme_release(ctx);
      throw_gpgme_error("gpgme_set_protocol failed", gpgme_strerror(err));
    }
    
    // %no-protection keeps the test key unprotected so generation is non-interactive.
    const char* params =
        "<GnupgKeyParms format=\"internal\">\n"
        "Key-Type: RSA\n"
        "Key-Length: 2048\n"
        "Subkey-Type: RSA\n"
        "Subkey-Length: 2048\n"
        "Name-Real: CK Test\n"
        "Name-Email: ck-test@example.com\n"
        "Expire-Date: 0\n"
        "%no-protection\n"
        "%commit\n"
        "</GnupgKeyParms>\n";
    
    err = gpgme_op_genkey(ctx, params, nullptr, nullptr);
    if (err) {
      gpgme_release(ctx);
      throw_gpgme_error("gpgme_op_genkey failed", gpgme_strerror(err));
    }
    
    gpgme_genkey_result_t result = gpgme_op_genkey_result(ctx);
    if (result == nullptr || result->fpr == nullptr) {
      gpgme_release(ctx);
      throw_gpgme_error("gpgme_op_genkey_result missing fingerprint", gpgme_strerror(err));
    }
    
    std::string fpr = result->fpr;
    gpgme_release(ctx);
    return fpr;
  }
}
