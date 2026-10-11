#include "ui/term.hpp"

#include "./internal/theme.hpp"

namespace ck::index {
  using namespace ck::ui::term;
  using enum ck::ui::term::Color;
  using enum ck::ui::term::Style;
  using enum OutputComponent;
  
  TextStyle get_scheme_style(OutputComponent c) {
    switch (c) {
      case CryptName:
      case NodeName:  
        return { Blue, { Bold } };
      case EntryName: 
        return { Yellow, { Faint } };
      case RootMountPath:
      case RootMountAlias:  
        return { Green, { Bold } };
      case RootMountArrow:  
        return { Orange, { Bold } };
      case MountPath:
      case MountAlias:      
        return { Aqua, { Bold } };
      case MountArrow:      
        return { Orange, { Bold } };
      case MountSubDir:     
        return { Blue, { Bold } };
      case Line:            
        return { Gray, { Faint } };
    }
    return { Gray };
  }
  
  std::string get_scheme_ansi(OutputComponent c) {
    return ansi(get_scheme_style(c));
  }
}
