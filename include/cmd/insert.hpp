#pragma once

#include "lib/types.hpp"

namespace ck::insert {
  void insert(ck::config::Config&, ck::config::Vault&, const ck::secret::Secret&);
}