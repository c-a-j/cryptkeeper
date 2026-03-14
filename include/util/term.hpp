#pragma once

#include <string>

namespace ck::util::term {
  enum class Color {
    Default,
    Gray,
    Red,
    Green,
    Yellow,
    Blue,
    Aqua,
    Orange,
    Purple,
    Reset
  };

  inline const char* code(Color c) {
    switch (c) {
      case Color::Gray:       return "38;2;146;131;116";
      case Color::Red:        return "38;2;251;73;52";
      case Color::Green:      return "38;2;184;187;38";
      case Color::Yellow:     return "38;2;250;189;47";
      case Color::Blue:       return "34";
      case Color::Aqua:       return "38;2;142;192;124";
      case Color::Orange:     return "38;2;254;128;25";
      case Color::Purple:     return "38;2;211;134;155";
      case Color::Reset:      return "0";
      case Color::Default:    return "";
    }
    return "";
  }

  inline std::string ansi(Color c, bool bold = false) {
    if (c == Color::Default) { return ""; }
    std::string out = "\033[";
    if (bold && c != Color::Reset) {
      out += "1;";
    }
    out += code(c);
    out += "m";
    return out;
  }
}
