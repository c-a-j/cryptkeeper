#include "lib/mount/types.hpp"
#include "../path/path.hpp"
#include "../path/existence.hpp"

namespace ck::mount {
  Mounts::Mounts() {
    this->file_ = ck::path::mount_file();
  }

  Mount& Mounts::root() {
    return this->root_;
  }
  
  const Mount& Mounts::root() const {
    return this->root_;
  }

  std::unordered_map<std::string, Mount>& Mounts::mounts() {
    return this->mounts_;
  }
  
  const std::unordered_map<std::string, Mount>& Mounts::mounts() const {
    return this->mounts_;
  }

  bool Mounts::empty() const noexcept {
    return (this->root_.path.empty() && this->mounts_.empty());
  }
  
  bool Mounts::file_exists() const noexcept {
    return (ck::path::file_exists(this->file_));
  }

  Mounts mnt;
}
