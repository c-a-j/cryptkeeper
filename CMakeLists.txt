cmake_minimum_required(VERSION 3.16)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(crypt-keeper VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(ck
    src/main.cpp
    src/cli/build_cli.cpp
    src/utils/logger.cpp
    src/commands/init.cpp
    src/commands/get.cpp
    src/commands/config.cpp
)

include(FetchContent)

FetchContent_Declare(
    CLI11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.3.0
)

FetchContent_MakeAvailable(CLI11)
target_link_libraries(ck PRIVATE CLI11::CLI11)
target_include_directories(ck PRIVATE include)