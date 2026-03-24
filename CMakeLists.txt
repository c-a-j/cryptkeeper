cmake_minimum_required(VERSION 3.16)
project(crypt-keeper VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(CK_WARNINGS_AS_ERRORS "Treat warnings as errors" ON)
option(CK_ENABLE_SANITIZERS "Enable ASan/UBSan in Debug (GCC/Clang)" ON)
option(CK_ENABLE_DEBUG_LOGS "Enable debug logging" ${CK_ENABLE_DEBUG_LOGS_DEFAULT})

set (CK_ENABLE_DEGUB_LOGS_DEFAULT OFF)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CK_ENABLE_DEBUG_LOGS_DEFAULT ON)
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(GPGME REQUIRED gpgme)

include(FetchContent)
FetchContent_Declare(
  CLI11 
  GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git 
  GIT_TAG v2.6.2
)
FetchContent_MakeAvailable(CLI11)

FetchContent_Declare(
  tomlplusplus
  GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git 
  GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(tomlplusplus)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(CompilerOptions)

add_subdirectory(src)

ck_apply_compiler_options(ck)
ck_apply_compiler_options(ck_lib)

include(CTest)
if (BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()
