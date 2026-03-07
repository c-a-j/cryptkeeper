#include <filesystem>
#include <stdexcept>
#include <gpgme.h>
#include <gtest/gtest.h>

#include "global.hpp"
#include "scoped_config_dir.hpp"


namespace ck::tests::util {
  namespace fs = std::filesystem;
  ScopedConfigDir::ScopedConfigDir() {
    char tmpl[] = "/tmp/crypt-keeper-XXXXXX";
    char* dir = mkdtemp(tmpl);
    if (dir == nullptr) {
      throw std::runtime_error("mkdtemp failed");
    }
    tmp_dir_ = dir;
    
    if (const char* old = std::getenv("XDG_DATA_HOME")) {
      dir_ = old;
    }
    if (setenv(VAULT_DIR_ENV_VAR.data(), tmp_dir_.c_str(), 1) != 0) {
      throw std::runtime_error("setenv(VAULT_DIR_ENV_VAR) failed");
    }
  }
  ScopedConfigDir::~ScopedConfigDir() {
    if (dir_.has_value()) {
      setenv(VAULT_DIR_ENV_VAR.data(), dir_->c_str(), 1);
    } else {
      unsetenv(VAULT_DIR_ENV_VAR.data());
    }
    std::error_code ec;
    fs::remove_all(tmp_dir_, ec);
  }
  
}