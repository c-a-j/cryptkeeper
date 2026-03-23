#include "cli/types.hpp"
#include "./_internal.hpp"

namespace ck::cmd {
  int run_command(const ck::cli::CliArgs& args) {
    ck::cli::CmdArgs cargs = args.cmd;
    ck::cli::Context ctx = {
      .root_args = args.root,
    };
    
    return std::visit([&](const auto& args) -> int {
      using T = std::decay_t<decltype(args)>;
      
      if constexpr (std::is_same_v<T, std::monostate>) {
        return 0;
      } else if constexpr (std::is_same_v<T, ck::cli::InitArgs>) {
        init(ctx, args);
      } else if constexpr (std::is_same_v<T, ck::cli::ConfigArgs>) {
        config(ctx, args);
      } else if constexpr (std::is_same_v<T, ck::cli::InsertArgs>) {
        insert(ctx, args);
      } else if constexpr (std::is_same_v<T, ck::cli::ShowArgs>) {
        show(ctx, args);
      } else if constexpr (std::is_same_v<T, ck::cli::MountArgs>) {
        mount(ctx, args);
      } else if constexpr (std::is_same_v<T, ck::cli::UmountArgs>) {
        umount(ctx, args);
      } else if constexpr (std::is_same_v<T, ck::cli::ChrootArgs>) {
        chroot(ctx, args);
      }
      return 0;
    }, cargs);
  }
}
