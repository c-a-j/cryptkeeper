add_executable(init_test
  init_test.cpp
  ../util/gen_key.cpp
  ../util/scoped_config_dir.cpp
  ../util/scoped_vault_root.cpp
  ${PROJECT_SOURCE_DIR}/src/cmd/init.cpp
  ${PROJECT_SOURCE_DIR}/src/util/logger.cpp
)

target_link_libraries(init_test PRIVATE
  ck_lib
  GTest::gtest_main
)

target_include_directories(init_test PRIVATE
  ${PROJECT_SOURCE_DIR}/include
)

gtest_discover_tests(init_test
  EXTRA_ARGS --gtest_print_time=0 --gtest_brief=1
)
