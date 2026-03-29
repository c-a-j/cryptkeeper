#include <deque>

#include "util/logger/logger.hpp"
#include "cli/types.hpp"
#include "../lib/path/parse_path.hpp"
#include "lib/mount/types.hpp"
#include "lib/index/types.hpp"

namespace {
  bool mount_is_child(
      const std::string alias, 
      const std::string resolved_alias, 
      const std::string relative_path
  ) {
    std::string resolved = relative_path + "/" + resolved_alias;
    std::vector<std::string> resolved_parts = ck::path::parse_path(resolved);
    std::vector<std::string> alias_parts = ck::path::parse_path(alias);
    std::deque<std::string> q_resolved(resolved_parts.begin(), resolved_parts.end());
    std::deque<std::string> q_alias(alias_parts.begin(), alias_parts.end());
    
    while (!q_resolved.empty() && !q_alias.empty()) {
      auto r = q_resolved.front();
      auto a = q_alias.front();
      if (r == a) {
        q_resolved.pop_front();
        q_alias.pop_front();
        continue;
      }
      return false;
    }
    return true;
  }
}

namespace ck::cmd {
  using ck::util::logger::logger;
  using ck::mount::mnt;
  
  void show(const ck::cli::Context& _, const ck::cli::ShowArgs& args){
    // resolve the path
    // only generate the index needed

    // if printing from root vault, generate an index for every mount
    // root nodes will need to be inserted in their alias locations for printing

    mnt.load();
    

    if (!args.path) {
      logger.debug("Printing the whole tree");
      logger.debug("path", mnt.root().path);
      
      ck::index::Index idx(mnt.root().path);
      for (auto [alias, mount] : mnt.mounts()) {
        ck::index::Index i(mount.path);
        idx.insert_node(i.root(), alias);
      }
      idx.print();
    } else {
      logger.debug("Printing the subtree tree or secret");
      
      ck::mount::ResolvedPath rp = mnt.resolve(*args.path);
      ck::index::Index idx(rp.vault_path, rp.alias);

      if (rp.relative_path.empty()) {
        idx.print(false);
        return;
      }

      for (auto [alias, mount] : mnt.mounts()) {
        if (mount_is_child(alias, rp.alias, rp.relative_path)) {
          ck::index::Index i(mount.path);
          idx.insert_node(i.root(), alias);
        }
      }
      idx.print(rp.relative_path);
    }
  }
}
