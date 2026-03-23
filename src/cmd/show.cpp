#include "util/logger/logger.hpp"
#include "cli/types.hpp"
#include "lib/mount/types.hpp"
#include "lib/index/types.hpp"

namespace ck::cmd {
  using ck::util::logger::logger;
  using ck::mount::mnt;
  
  void show(const ck::cli::Context& _, const ck::cli::ShowArgs& args){
    // resolve the path
    // only generate the index needed

    // if printing from root vault, generate an index for every mount
    // root nodes will need to be inserted in their alias locations for printing

    mnt.deserialize();

    if (!args.path) {
      logger.info("Printing the whole tree");
      ck::index::Index root(mnt.root().path);
      root.print();
    } else {
      ck::mount::ResolvedPath rp = mnt.resolve(*args.path);
      ck::index::Index root(mnt.root().path);
      root.print(*args.path);
      logger.info("Printing a subtree or secret");
    }
  }
}
