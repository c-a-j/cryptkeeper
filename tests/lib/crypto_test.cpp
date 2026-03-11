#include <gpgme.h>
#include <gtest/gtest.h>

#include "lib/crypto/crypto.hpp"
#include "../util/gen_key.hpp"

using namespace ck::crypto;
using namespace ck::tests::util;

class CryptoTest : public ::testing::Test {
  protected: 
    std::unique_ptr<ScopedGnupgHome> gnupg_home_;
    std::string generated_fpr_;

    void SetUp() override {
      gnupg_home_ = std::make_unique<ScopedGnupgHome>();
      init_gpgme();
      generated_fpr_ = generate_tmp_key();
    } 
    void TearDown() override {
      gnupg_home_.reset();
    } 
};

TEST_F(CryptoTest, InitGgpmeSucceeds) {
  EXPECT_NO_THROW(init_gpgme());
}

TEST_F(CryptoTest, KeyExistsReturnTrue) {
  EXPECT_TRUE(key_exists(generated_fpr_, true));
}

TEST_F(CryptoTest, KeyNotExistsReturnFalse) {
  std::string fpr = generated_fpr_;
  gnupg_home_.reset();
  EXPECT_FALSE(key_exists(fpr, true));
}
