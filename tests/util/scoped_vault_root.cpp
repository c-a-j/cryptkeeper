#include <filesystem>
#include <stdexcept>
#include <gpgme.h>
#include <gtest/gtest.h>

#include "scoped_vault_root.hpp"
#include "global.hpp"


namespace ck::tests::util {
  namespace fs = std::filesystem;
  ScopedVaultRoot::ScopedVaultRoot() {
    char tmpl[] = "/tmp/crypt-keeper-XXXXXX";
    char* dir = mkdtemp(tmpl);
    if (dir == nullptr) {
      throw std::runtime_error("mkdtemp failed");
    }
    tmp_root_ = dir;
    
    if (const char* old = std::getenv(VAULT_DIR_ENV_VAR.data())) {
      root_ = old;
    }
    if (setenv("XDG_DATA_HOME", tmp_root_.c_str(), 1) != 0) {
      throw std::runtime_error("setenv(VAULT_DIR_ENV_VAR) failed");
    }
  }
  ScopedVaultRoot::~ScopedVaultRoot() {
    if (root_.has_value()) {
      setenv(VAULT_DIR_ENV_VAR.data(), root_->c_str(), 1);
    } else {
      unsetenv(VAULT_DIR_ENV_VAR.data());
    }
    std::error_code ec;
    fs::remove_all(tmp_root_, ec);
  }
  
}