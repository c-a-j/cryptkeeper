#ifndef _WIN32

#include <string>
#include <filesystem>
#include <vector>
#include <unistd.h>
#include <cerrno>
#include "util/error.hpp"

#include "./tempfile.hpp"
#include "./write_all.hpp"
#include "./msg.hpp"

namespace ck::fio {
  namespace fs = std::filesystem;
  using ck::util::error::Error;
  using ck::util::error::FsErrc;
  using enum ck::util::error::FsErrc;
    
  TempFile::TempFile(const fs::path& target_) : target(target_) {
    std::string tmpl = target.string() + ".tmp.XXXXXX";
    std::vector<char> buf(tmpl.begin(), tmpl.end());
    buf.push_back('\0');
    fd = ::mkstemp(buf.data());
    if (fd == -1) {
      throw Error<FsErrc>{MkstempFailed, msg(target, errno)};
    }
    
    try {
      path = buf.data();
    } catch (const std::exception& e) {
      ::close(fd);
      ::unlink(buf.data());
      fd = -1;
      throw Error<FsErrc>{ FileSystemError, e.what() };
    }
  };
    
  int TempFile::write(std::span<const std::byte> bytes) noexcept {
    return write_all(
      fd, 
      reinterpret_cast<const char*>(bytes.data()),
      bytes.size()
    );
  };

  int TempFile::sync() noexcept {
    if (::fsync(fd) == -1) {
      return errno;
    }
    return 0;
  }

  int TempFile::close() noexcept { 
    if (fd == -1) { return 0; }
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
}

#endif
