#include "cli/types.hpp"
#include "lib/mount/mount.hpp"

namespace ck::cmd {
using ck::mount::mnt;
void umount(const ck::cli::Context &_, const ck::cli::UmountArgs &args) {
    if (args.alias) {
        mnt.umount(*args.alias);
    }
};
} // namespace ck::cmd
