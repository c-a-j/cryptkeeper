#include "cli/types.hpp"

#include "./internal/handlers.hpp"
#include "lib/index/index.hpp"
#include "lib/mount/mount.hpp"
#include "ui/console/console.hpp"

namespace ck::cmd {
using ck::mount::mnt;
using ck::ui::console::console;
void insert(const ck::cli::Context &_, const ck::cli::InsertArgs &args) {
    console.debug("Inserting a new secret");
    ck::mount::ResolvedPath rp = mnt.resolve(args.path);

    ck::index::Index idx(rp.crypt_path);
    idx.insert(rp.relative_path, args.pwgen);
}
} // namespace ck::cmd
