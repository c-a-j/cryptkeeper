#include <unistd.h>
#include <cerrno>
#include <cstring>
#include "util/error.hpp"

namespace ck::fio {
  using enum ck::util::error::FsErrc;
  
  int write_all(int fd, const char* data, std::size_t len) {
    std::size_t written = 0;
    while (written < len) {
      ssize_t n = ::write(fd, data + written, len - written);
      if (n > 0) {
        written += static_cast<std::size_t>(n);
        continue;
      }
      if (n == -1) {
        if (errno == EINTR) { continue;}
        return errno;
      }
      return EIO;
    }
    return 0;
  }
}
