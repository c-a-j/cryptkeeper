#include "lib/mount/types.hpp"
#include "../path/path.hpp"
#include "../path/existence.hpp"

namespace ck::mount {
  Mounts::Mounts() {
    this->file_ = ck::path::mount_file();
  }

  Mounts::Mounts(State& state) : state_(std::move(state))  {
    this->file_ = ck::path::mount_file();
  }

  Mount& Mounts::root() {
    return this->state_.root;
  }
  
  const Mount& Mounts::root() const {
    return this->state_.root;
  }

  std::unordered_map<std::string, Mount>& Mounts::mounts() {
    return this->state_.mounts;
  }
  
  const std::unordered_map<std::string, Mount>& Mounts::mounts() const {
    return this->state_.mounts;
  }

  bool Mounts::empty() const noexcept {
    return (this->state_.root.path.empty() && this->state_.mounts.empty());
  }
  
  bool Mounts::file_exists() const noexcept {
    return (ck::path::file_exists(this->file_));
  }

  Mounts mnt;
}
