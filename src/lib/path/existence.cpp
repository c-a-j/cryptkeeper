#include <filesystem>
#include <string>

#include "lib/path/error.hpp"

namespace {
bool missing_path(const std::error_code &ec) {
    return ec == std::errc::no_such_file_or_directory || ec == std::errc::not_a_directory;
}
} // namespace

namespace ck::path {
namespace fs = std::filesystem;
using ck::error::Error;
using ck::error::PathErrc;
using enum ck::error::PathErrc;

bool exists(const fs::path &path) {
    std::error_code ec;
    bool exists = fs::exists(path, ec);
    if (missing_path(ec)) {
        return false;
    }
    if (ec) {
        throw Error<PathErrc>{
            FileSystemError, path.string() + ": " + ec.message()};
    }
    return exists;
}

bool exists_or_throw(const fs::path &path) {
    bool exists = ck::path::exists(path);
    if (!exists) {
        throw Error<PathErrc>{DoesNotExist, path.string()};
    }
    return exists;
}

bool file_exists(const fs::path &path) {
    std::error_code ec;
    bool exists = fs::is_regular_file(path, ec);
    if (missing_path(ec)) {
        return false;
    }
    if (ec) {
        throw Error<PathErrc>{
            FileSystemError, path.string() + ": " + ec.message()};
    }
    return exists;
}

bool file_exists_or_throw(const fs::path &path) {
    bool exists = file_exists(path);
    if (!exists) {
        throw Error<PathErrc>{FileDoesNotExist, path.string()};
    }
    return exists;
}

bool directory_exists(const fs::path &path) {
    std::error_code ec;
    bool exists = fs::is_directory(path, ec);
    if (missing_path(ec)) {
        return false;
    }
    if (ec) {
        throw Error<PathErrc>{
            FileSystemError, path.string() + ": " + ec.message()};
    }
    return exists;
}

bool directory_exists_or_throw(const fs::path &path) {
    bool exists = directory_exists(path);
    if (!exists) {
        throw Error<PathErrc>{DirectoryDoesNotExist, path.string()};
    }
    return exists;
}
} // namespace ck::path
