#include <utility>

#include "lib/mount/mount.hpp"
#include "lib/path/existence.hpp"

#include "../path/internal/path.hpp"

namespace ck::mount {
Mounts::Mounts(State &state) : state_(std::move(state)) {
}

Mount &Mounts::root() {
    return this->state_.root;
}

const Mount &Mounts::root() const {
    return this->state_.root;
}

std::unordered_map<std::string, Mount> &Mounts::mounts() {
    return this->state_.mounts;
}

const std::unordered_map<std::string, Mount> &Mounts::mounts() const {
    return this->state_.mounts;
}

bool Mounts::empty() const noexcept {
    return (this->state_.root.path.empty() && this->state_.mounts.empty());
}

bool Mounts::file_exists() {
    return (ck::path::file_exists(ck::path::mount_file()));
}

Mounts mnt;
} // namespace ck::mount
