#include <gtest/gtest.h>

#include "cmd/init.hpp"
#include "../util/gen_key.hpp"
#include "../util/scoped_vault_root.hpp"
#include "../util/scoped_config_dir.hpp"
#include "lib/crypto.hpp"
#include "util/error.hpp"

using namespace ck::cmd::init;
using namespace ck::lib::crypto;
using namespace ck::tests::util;

class InitTest : public ::testing::Test {
  protected: 
    std::unique_ptr<ScopedGnupgHome> gnupg_home_;
    std::unique_ptr<ScopedVaultRoot> store_root_;
    std::unique_ptr<ScopedConfigDir> config_dir_;
    Vault vault_;
    Config cfg_;
    
    void SetUp() override {
      gnupg_home_ = std::make_unique<ScopedGnupgHome>();
      init_gpgme();
      store_root_ = std::make_unique<ScopedVaultRoot>();
      config_dir_ = std::make_unique<ScopedConfigDir>();
      vault_.name = "test-vault";
      vault_.key_fpr = generate_tmp_key();
    }
    void TearDown() override {
      gnupg_home_.reset();
      store_root_.reset();
    } 
};

TEST_F(InitTest, InitNewVaultNoThrow) {
  EXPECT_NO_THROW(init_vault(cfg_, vault_));
}

TEST_F(InitTest, InitExistingStoreThrows) {
  init_vault(cfg_, vault_);
  EXPECT_THROW(init_vault(cfg_, vault_), ck::util::error::AppError);
}

// TEST_F(CryptoTest, KeyExistsTrueWhenKeyExists) {
//   EXPECT_TRUE(key_exists(generated_fpr_, true));
// }
