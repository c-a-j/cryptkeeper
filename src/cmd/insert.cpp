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
    ck::mount::ResolvedPath rp = mnt.resolve(args.path);
    logger.info("alias", std::string(rp.alias));
    logger.info("vault_path", rp.vault_path.string());
    logger.info("relative_path", std::string(rp.relative_path));
    ck::index::Index idx(rp.vault_path);
    idx.insert(rp.relative_path, args.pwgen);
  }
}
