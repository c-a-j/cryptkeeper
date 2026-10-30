#include "scoped_crypt_root.hpp"

#include <filesystem>
#include <stdexcept>

#include <gpgme.h>
#include <gtest/gtest.h>

#include "global.hpp"

namespace ck::tests::util {
namespace fs = std::filesystem;
ScopedCryptRoot::ScopedCryptRoot() {
    char tmpl[] = "/tmp/crypt-keeper-XXXXXX";
    char *dir = mkdtemp(tmpl);
    if (dir == nullptr) {
        throw std::runtime_error("mkdtemp failed");
    }
    tmp_root_ = dir;

    if (const char *old = std::getenv(CRYPT_DIR_ENV_VAR.data())) {
        root_ = old;
    }
    if (setenv("XDG_DATA_HOME", tmp_root_.c_str(), 1) != 0) {
        throw std::runtime_error("setenv(CRYPT_DIR_ENV_VAR) failed");
    }
}
ScopedCryptRoot::~ScopedCryptRoot() {
    if (root_.has_value()) {
        setenv(CRYPT_DIR_ENV_VAR.data(), root_->c_str(), 1);
    } else {
        unsetenv(CRYPT_DIR_ENV_VAR.data());
    }
    std::error_code ec;
    fs::remove_all(tmp_root_, ec);
}

} // namespace ck::tests::util