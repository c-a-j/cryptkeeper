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