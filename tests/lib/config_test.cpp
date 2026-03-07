#include <gtest/gtest.h>

#include "../util/scoped_config_dir.hpp"
#include "util/error.hpp"

using namespace ck::tests::util;

class InitTest : public ::testing::Test {
  protected: 
    std::unique_ptr<ScopedConfigDir> config_dir_;
    
    void SetUp() override {
      config_dir_ = std::make_unique<ScopedConfigDir>();
    }
    void TearDown() override {
      config_dir_.reset();
    } 
};

// TEST_F(InitTest, InitNewVaultNoThrow) {
//   EXPECT_NO_THROW(init_vault("test-store", generated_fpr_));
// }

// TEST_F(InitTest, InitExistingStoreThrows) {
//   init_vault("test-store", generated_fpr_);
//   EXPECT_THROW(init_vault("test-store", generated_fpr_), ck::util::error::AppError);
// }

// TEST_F(CryptoTest, KeyExistsTrueWhenKeyExists) {
//   EXPECT_TRUE(key_exists(generated_fpr_, true));
// }