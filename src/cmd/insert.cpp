#include "util/logger/logger.hpp"
#include "cli/types.hpp"
#include "lib/config/types.hpp"
#include "lib/index/types.hpp"
#include "lib/mount/types.hpp"
#include "./_internal.hpp"

namespace ck::cmd {
  using ck::util::logger::logger;
  using ck::mount::mnt;
  void insert(const ck::cli::Context& _, const ck::cli::InsertArgs& args) {
    logger.debug("Inserting a new secret");
    ck::mount::ResolvedPath rp = mnt.resolve(args.path);
    ck::index::Index idx(rp.vault_path);
    idx.insert(rp.relative_path, args.pwgen);
  }
}
