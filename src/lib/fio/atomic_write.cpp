#include <filesystem>
#include <string>

#include "lib/fio/error.hpp"

#include "./internal/msg.hpp"
#include "./internal/sync_dir.hpp"
#include "./internal/tempfile.hpp"

namespace ck::fio {
namespace fs = std::filesystem;
using ck::error::Error;
using ck::error::FsErrc;
using enum ck::error::FsErrc;

void atomic_write(const fs::path &target, std::span<const std::byte> bytes) {
    std::error_code ec;
    fs::path parent = target.parent_path();
    if (!parent.empty()) {
        fs::create_directories(parent, ec);
    }
    if (ec) {
        throw Error<FsErrc>{
            CreateDirectoryFailed, parent.string() + ": " + ec.message()};
    }

    TempFile tmp(target);
    int err = tmp.write(bytes);
    if (err != 0) {
        throw Error<FsErrc>{WriteFailed, msg(tmp.path, err)};
    }

    err = tmp.sync();
    if (err != 0) {
        throw Error<FsErrc>{FsyncFailed, msg(tmp.path, err)};
    }

    err = tmp.close();
    if (err != 0) {
        throw Error<FsErrc>{CloseFailed, msg(tmp.path, err)};
    }

    err = tmp.commit();
    if (err != 0) {
        throw Error<FsErrc>{
            RenameFailed, msg(tmp.path, err) + " -> " + target.string()};
    }

    sync_dir(parent);
}

void atomic_write(const fs::path &target, const std::string &contents) {
    std::span<const char> chars(contents.data(), contents.size());
    std::span<const std::byte> bytes = std::as_bytes(chars);
    atomic_write(target, bytes);
}
} // namespace ck::fio
