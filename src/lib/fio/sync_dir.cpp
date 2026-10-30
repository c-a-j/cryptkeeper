#include <fcntl.h>
#include <unistd.h>

#include <filesystem>

#include "./internal/msg.hpp"
#include "lib/fio/error.hpp"

namespace ck::fio {
namespace fs = std::filesystem;
using ck::error::Error;
using ck::error::FsErrc;
using enum ck::error::FsErrc;

void sync_dir(const fs::path &path) {
#ifndef WIN32_
    int fd = ::open(path.c_str(), O_RDONLY | O_DIRECTORY);
    if (fd == -1) {
        throw Error<FsErrc>{OpenDirectoryFailed, msg(path, errno)};
    }
    if (::fsync(fd) == -1) {
        throw Error<FsErrc>{SyncDirectoryFailed, msg(path, errno)};
    }
    ::close(fd);
#else
    return;
#endif
}
} // namespace ck::fio
