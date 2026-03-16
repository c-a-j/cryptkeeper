#pragma once

#include "lib/types.hpp"

namespace ck::show {
  void show(ck::config::Config&, ck::config::Vault&, const ck::secret::Secret&);
}