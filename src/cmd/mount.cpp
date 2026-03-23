#include "cli/types.hpp"
#include "lib/mount/types.hpp"

namespace ck::cmd {
  using ck::mount::mnt;

  void mount(const ck::cli::Context& _, const ck::cli::MountArgs& args) {
    bool list = args.list;

    if (!args.alias && !args.path) { list = true; }
    if (list) { mnt.print(); return; }

    if (args.alias && !args.path) {
      // alias is expected to be a directory in vault home
      // if vault_home/alias is not found, throw

      // ck mount foo
      // this should attemt to mount the actual directory vault_home/foo with 
      // with the alias foo
      mnt.mount(*args.alias);
      return;
    } 
    // if path is a single component, it should be interpreted as pointing to 
    // a directory relative to vault home

    // ck mount foo/bar foo
    // this should attemt to mount the actual directory vault_home/foo with 
    // with the alias foo/bar

    // ck mount foo/bar /any/directory
    // this should attemt to mount the actual directory /any/directory with 
    // with the alias foo/bar

    mnt.mount(*args.alias, *args.path);
  };
}
