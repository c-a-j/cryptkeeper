#include "cli/types.hpp"
#include <gtest/gtest.h>

#include "../util/gen_key.hpp"
#include "../util/scoped_config_dir.hpp"
#include "../util/scoped_crypt_root.hpp"
#include "cmd/_internal.hpp"
#include "lib/crypto/crypto.hpp"

using namespace ck;
using namespace ck::tests::util;

class InitTest : public ::testing::Test {
  protected:
    std::unique_ptr<ScopedGnupgHome> gnupg_home_;
    std::unique_ptr<ScopedCryptRoot> store_root_;
    std::unique_ptr<ScopedConfigDir> config_dir_;
    cli::Context ctx_;
    cli::InitArgs args_;

    void SetUp() override {
        gnupg_home_ = std::make_unique<ScopedGnupgHome>();
        crypto::init_gpgme();
        store_root_ = std::make_unique<ScopedCryptRoot>();
        config_dir_ = std::make_unique<ScopedConfigDir>();
    }
    void TearDown() override {
        gnupg_home_.reset();
        store_root_.reset();
    }
};

TEST_F(InitTest, InitNewCryptNoThrow) {
    args_.crypt_name = "test-crypt";
    args_.key_fpr = generate_tmp_key();
    EXPECT_NO_THROW(ck::cmd::init(ctx_, args_));
}

TEST_F(InitTest, InitExistingStoreThrows) {
    args_.crypt_name = "test-crypt";
    args_.key_fpr = generate_tmp_key();
    ck::cmd::init(ctx_, args_);
    EXPECT_THROW(ck::cmd::init(ctx_, args_), ck::error::AppError);
}

TEST_F(InitTest, InitStoreEmptyArgsThrows) {
    EXPECT_THROW(ck::cmd::init(ctx_, args_), ck::error::AppError);
}
