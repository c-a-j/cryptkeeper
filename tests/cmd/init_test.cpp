#include <gtest/gtest.h>

#include "cmd/_internal.hpp"
#include "lib/crypto/crypto.hpp"
#include "cli/types.hpp"

#include "../util/gen_key.hpp"
#include "../util/scoped_vault_root.hpp"
#include "../util/scoped_config_dir.hpp"

using namespace ck;
using namespace ck::tests::util;

class InitTest : public ::testing::Test {
  protected: 
    std::unique_ptr<ScopedGnupgHome> gnupg_home_;
    std::unique_ptr<ScopedVaultRoot> store_root_;
    std::unique_ptr<ScopedConfigDir> config_dir_;
    cli::Context ctx_;
    cli::InitArgs args_;
    
    void SetUp() override {
      gnupg_home_ = std::make_unique<ScopedGnupgHome>();
      crypto::init_gpgme();
      store_root_ = std::make_unique<ScopedVaultRoot>();
      config_dir_ = std::make_unique<ScopedConfigDir>();
      
      args_.vault_name = "test-vault";
      args_.key_fpr = generate_tmp_key();
    }
    void TearDown() override {
      gnupg_home_.reset();
      store_root_.reset();
    } 
};

TEST_F(InitTest, InitNewVaultNoThrow) {
  EXPECT_NO_THROW(ck::cmd::init(ctx_, args_));
}

TEST_F(InitTest, InitExistingStoreThrows) {
  ck::cmd::init(ctx_, args_);
  EXPECT_THROW(ck::cmd::init(ctx_, args_), ck::util::error::AppError);
}