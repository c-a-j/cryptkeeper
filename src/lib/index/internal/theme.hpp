#pragma once

#include "ui/term.hpp"
#include <string>

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
ck::ui::term::TextStyle get_scheme_style(OutputComponent c);
std::string get_scheme_ansi(OutputComponent c);
} // namespace ck::index
