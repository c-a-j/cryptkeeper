#include <gtest/gtest.h>

#include "cmd/init.hpp"
#include "../util/gen_key.hpp"
#include "../util/scoped_store_root.hpp"
#include "lib/crypto.hpp"
#include "util/error.hpp"

using namespace ck::cmd::init;
using namespace ck::lib::crypto;
using namespace ck::tests::util;

class InitTest : public ::testing::Test {
  protected: 
    std::unique_ptr<ScopedGnupgHome> gnupg_home_;
    std::string generated_fpr_;
    std::unique_ptr<ScopedStoreRoot> store_root_;
    
    void SetUp() override {
      gnupg_home_ = std::make_unique<ScopedGnupgHome>();
      init_gpgme();
      generated_fpr_ = generate_tmp_key();
      store_root_ = std::make_unique<ScopedStoreRoot>();
    }
    void TearDown() override {
      gnupg_home_.reset();
      store_root_.reset();
    } 
};

TEST_F(InitTest, InitNewVaultNoThrow) {
  EXPECT_NO_THROW(init_vault("test-store", generated_fpr_));
}

TEST_F(InitTest, InitExistingStoreThrows) {
  init_vault("test-store", generated_fpr_);
  EXPECT_THROW(init_vault("test-store", generated_fpr_), ck::util::error::AppError);
}

// TEST_F(CryptoTest, KeyExistsTrueWhenKeyExists) {
//   EXPECT_TRUE(key_exists(generated_fpr_, true));
// }
