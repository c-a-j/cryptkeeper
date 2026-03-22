#include "cli/types.hpp"

#include "lib/index/find.hpp"

namespace ck::cmd {
  void show(const ck::cli::Context& ctx, const ck::cli::ShowArgs& args){
    ck::index::find(args);
  }
}