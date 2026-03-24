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

namespace ck::fs {
  namespace fs = std::filesystem;
  using ck::util::error::Error;
  using ck::util::error::PathErrc;
  using enum ck::util::error::PathErrc;
    
  TempFile::TempFile(const fs::path& target_) : target(target_) {
    std::string tmpl = target.string() + ".tmp.XXXXXX";
    std::vector<char> buf(tmpl.begin(), tmpl.end());
    buf.push_back('\0');
    fd = ::mkstemp(buf.data());
    if (fd == -1) {
      throw Error<PathErrc>{MkstempFailed, msg(target, errno)};
    }
    
    try {
      path = buf.data();
    } catch (const std::exception& e) {
      ::close(fd);
      ::unlink(buf.data());
      fd = -1;
      throw Error<PathErrc>{ FileSystemError, e.what() };
    }
  };
    
  int TempFile::write(const std::string& contents) noexcept {
    return write_all(fd, contents.data(), contents.size());
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
