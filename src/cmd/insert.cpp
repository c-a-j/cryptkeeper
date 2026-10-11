#include "util/logger/logger.hpp"
#include "cli/types.hpp"
#include "lib/index/index.hpp"
#include "lib/mount/mount.hpp"
#include "./internal/handlers.hpp"

namespace ck::cmd {
  using ck::util::logger::logger;
  using ck::mount::mnt;
  void insert(const ck::cli::Context& _, const ck::cli::InsertArgs& args) {
    logger.debug("Inserting a new secret");
    ck::mount::ResolvedPath rp = mnt.resolve(args.path);

    ck::index::Index idx(rp.crypt_path);
    idx.insert(rp.relative_path, args.pwgen);
  }
}
