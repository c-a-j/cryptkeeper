#include "cli/types.hpp"

#include "lib/mount/mount.hpp"

namespace ck::cmd {
using ck::mount::mnt;
void chroot(const ck::cli::Context &_, const ck::cli::ChrootArgs &args) {
    if (args.path) {
        mnt.chroot(*args.path);
    }
};
} // namespace ck::cmd
