#pragma once

#include <string>
#include "util/term.hpp"

namespace ck::index {
  enum class OutputComponent {
    CryptName,
    NodeName,
    EntryName,
    RootMountPath,
    RootMountAlias,
    RootMountArrow,
    MountPath,
    MountSubDir,
    MountAlias,
    MountArrow,
    Line
  };
  ck::util::term::TextStyle get_scheme_style(OutputComponent c);
  std::string get_scheme_ansi(OutputComponent c);
}
