add_library(ck_lib STATIC)
add_executable(ck main.cpp)

add_subdirectory(lib)
add_subdirectory(cli)
add_subdirectory(cmd)
add_subdirectory(util)

target_compile_features(ck PRIVATE cxx_std_23)
target_compile_features(ck_lib PUBLIC cxx_std_23)

target_include_directories(ck PRIVATE ${PROJECT_SRC_DIR}/include)
target_include_directories(ck_lib PUBLIC
  ${PROJECT_SOURCE_DIR}/include
  ${GPGME_INCLUDE_DIRS}
)
target_compile_definitions(ck PRIVATE
  CK_ENABLE_DEBUG_LOGS=$<BOOL:${CK_ENABLE_DEBUG_LOGS}>
)

target_link_libraries(ck PRIVATE
  ck_lib
  CLI11::CLI11
)
target_link_libraries(ck_lib PUBLIC
  ${GPGME_LIBRARIES}
  tomlplusplus::tomlplusplus
)
