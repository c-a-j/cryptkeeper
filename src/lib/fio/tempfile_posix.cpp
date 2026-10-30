#ifndef _WIN32

#include <unistd.h>

#include <cerrno>
#include <filesystem>
#include <string>
#include <vector>

#include "./internal/msg.hpp"
#include "./internal/tempfile.hpp"
#include "./internal/write_all.hpp"
#include "lib/fio/error.hpp"

namespace ck::fio {
namespace fs = std::filesystem;
using ck::error::Error;
using ck::error::FsErrc;
using enum ck::error::FsErrc;

TempFile::TempFile(fs::path target_) : target(std::move(target_)) {
    std::string tmpl = target.string() + ".tmp.XXXXXX";
    std::vector<char> buf(tmpl.begin(), tmpl.end());
    buf.push_back('\0');
    fd = ::mkstemp(buf.data());
    if (fd == -1) {
        throw Error<FsErrc>{MkstempFailed, msg(target, errno)};
    }

    try {
        path = buf.data();
    } catch (const std::exception &e) {
        ::close(fd);
        ::unlink(buf.data());
        fd = -1;
        throw Error<FsErrc>{FileSystemError, e.what()};
    }
};

int TempFile::write(std::span<const std::byte> bytes) const noexcept {
    return write_all(
        fd, reinterpret_cast<const char *>(bytes.data()), bytes.size()
    );
};

int TempFile::sync() const noexcept {
    if (::fsync(fd) == -1) {
        return errno;
    }
    return 0;
}

int TempFile::close() noexcept {
    if (fd == -1) {
        return 0;
    }
    int rc = ::close(fd);
    int err = (rc == -1) ? errno : 0;
    fd = -1;
    return err;
}

int TempFile::commit() noexcept {
    if (::rename(path.c_str(), target.c_str()) == -1) {
        return errno;
    }
    active = false;
    return 0;
};

TempFile::~TempFile() noexcept {
    close();
    if (active) {
        ::unlink(path.c_str());
    }
};
} // namespace ck::fio

#endif
