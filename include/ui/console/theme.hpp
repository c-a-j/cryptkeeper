#pragma once
#include <optional>

#include "ui/term.hpp"

namespace ck::ui::console {
  enum class Stream { Out, Err, Default };
  
  enum class Level {
    Info,
    Success,
    Warning,
    Error,
    Debug
  };
  
  struct Overrides {
    std::optional<ck::ui::term::TextStyle> style;
    std::optional<Stream> stream;
    bool prefix = true;
    bool newline = true;
    
    Overrides& set_color(const ck::ui::term::TextStyle& v) { style = v; return *this; }
    Overrides& set_stream(Stream v) { stream = v; return *this; }
    Overrides& set_prefix(bool v) { prefix = v; return *this; }
    Overrides& set_newline(bool v) { newline = v; return *this; }
  };
  
  struct ColorScheme {
    ck::ui::term::TextStyle primary;
    ck::ui::term::TextStyle secondary;
    ck::ui::term::TextStyle prefix;
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
  const LevelStyle& style_for(const Theme& t, Level l);

  const char* get_level_tag(Level l);
  Stream get_level_stream(Level l);
}
