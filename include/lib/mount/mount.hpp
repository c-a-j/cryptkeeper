#pragma once

#include "cli/types.hpp"

namespace ck::mount {
  void mount(const ck::cli::Context&, const ck::cli::MountArgs&);
}