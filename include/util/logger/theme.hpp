#pragma once
#include <optional>

#include "util/term.hpp"

namespace ck::util::logger {
  enum class Stream { Out, Err, Default };
  
  enum class Level {
    Info,
    Success,
    Warning,
    Error,
    Debug
  };
  
  struct Overrides {
    std::optional<ck::util::term::TextStyle> style;
    std::optional<Stream> stream;
    bool prefix = true;
    bool newline = true;
    
    Overrides& set_color(ck::util::term::TextStyle v) { style = v; return *this; }
    Overrides& set_stream(Stream v) { stream = v; return *this; }
    Overrides& set_prefix(bool v) { prefix = v; return *this; }
    Overrides& set_newline(bool v) { newline = v; return *this; }
  };
  
  struct ColorScheme {
    ck::util::term::TextStyle primary;
    ck::util::term::TextStyle secondary;
    ck::util::term::TextStyle prefix;
  };
  
  struct LevelStyle {
    ColorScheme colors;
    const char* tag;
    Stream stream = Stream::Out;
  };
  
  struct Theme {
    LevelStyle info, success, warning, error, debug;
  };
  
  const Theme& default_theme();
  const LevelStyle& style_for(const Theme&, Level);
}