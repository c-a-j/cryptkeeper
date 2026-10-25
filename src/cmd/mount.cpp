#include "lib/mount/mount.hpp"
#include "cli/types.hpp"
#include <CLI/CLI.hpp>

namespace ck::cmd {
using ck::mount::mnt;

void mount(const ck::cli::Context &_, const ck::cli::MountArgs &args) {
    bool list = args.list;

    if (!args.alias && !args.path) {
        list = true;
    }
    if (list) {
        mnt.print();
        return;
    }

    if (args.path && !args.alias) {
        // When one argument is given, args.path essentially serves as the crypt
        // path and alias. In this case the argument is expected to be a crypt
        // that exists in core.home. If crypt_home/args.path is not found, an
        // exception is thrown

        // example
        // ck mount foo
        // this will attemt to mount the actual directory core.home/foo with
        // with the alias foo
        if (args.path) {
            mnt.mount(*args.path);
        }
        return;
    }
    // if path is a single component, it should be interpreted as pointing to
    // a directory relative to crypt home

    // ck mount foo/bar foo
    // this should attemt to mount the actual directory crypt_home/foo with
    // with the alias foo/bar

    // ck mount foo/bar /any/directory
    // this should attemt to mount the actual directory /any/directory with
    // with the alias foo/bar

    if (args.path && args.alias) {
        mnt.mount(*args.path, *args.alias);
    }
};
} // namespace ck::cmd
