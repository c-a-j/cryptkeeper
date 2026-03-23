#include "util/term.hpp"

#include "./_internal/theme.hpp"

namespace ck::index {
  using namespace ck::util::term;
  using enum ck::util::term::Color;
  using enum ck::util::term::Style;
  using enum OutputComponent;
  
  TextStyle get_scheme_style(OutputComponent c) {
    switch (c) {
      case VaultName: return { Blue, { Bold } };
      case NodeName:  return { Blue, { Bold } };
      case EntryName: return { Yellow, { Faint } };
      case RootMountPath:   return { Green, { Bold } };
      case RootMountAlias:  return { Green, { Bold } };
      case RootMountArrow:  return { Orange, { Bold } };
      case MountPath:       return { Aqua, { Bold } };
      case MountAlias:      return { Aqua, { Bold } };
      case MountArrow:      return { Orange, { Bold } };
      case MountSubDir:     return { Blue, { Bold } };
      case Line:            return { Gray, { Faint } };
    }
    return { Gray };
  }
  
  std::string get_scheme_ansi(OutputComponent c) {
    return ansi(get_scheme_style(c));
  }
}
