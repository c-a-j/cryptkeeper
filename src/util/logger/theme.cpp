#include "util/logger/theme.hpp"
#include "util/term.hpp"

namespace ck::util::logger {
  using namespace ck::util::term;
  using enum Level;
  using enum Color;
  using enum Style;
  
  TextStyle get_primary_style(Level level) {
    switch (level) {
      case Info:    return { Gray,   { Normal } };
      case Success: return { Green,  { Normal } };
      case Warning: return { Yellow, { Normal } };
      case Error:   return { Red,    { Normal } };
      case Debug:   return { Yellow, { Normal } };
    }
  }
  
  TextStyle get_secondary_style() {
    return { Gray, { Normal } };
  }
  
  TextStyle get_prefix_style() {
    return { Purple, { Normal } };
  }
  
  const char* get_level_tag(Level level) {
    switch (level) {
      case Info:    return "[.]";
      case Success: return "[*]";
      case Warning: return "[W]";
      case Error:   return "[E]";
      case Debug:   return "[D]";
    }
  }
  
  Stream get_level_stream(Level level) {
    switch (level) {
      case Error:   return Stream::Err;
      case Debug:   return Stream::Err;
      default:      return Stream::Out;
    }
  }
  
  ColorScheme get_color_scheme(Level level) {
    ColorScheme cs {
      .primary = get_primary_style(level),
      .secondary = get_secondary_style(),
      .prefix = get_prefix_style()
    };
    return cs;
  }
  
  LevelStyle get_level_style(Level level) {
    LevelStyle ls {
      .colors = get_color_scheme(level),
      .tag = get_level_tag(level),
      .stream = get_level_stream(level)
    };
    return ls;
  }
  
  
  const Theme& default_theme() {
    static const Theme theme{
      .info = get_level_style(Info),
      .success = get_level_style(Success),
      .warning = get_level_style(Warning),
      .error = get_level_style(Error),
      .debug = get_level_style(Debug)
    };
    return theme;
  }
  
  const LevelStyle& style_for(const Theme& t, Level l) {
    switch (l) {
      case Level::Info:     return t.info;
      case Level::Success:  return t.success;
      case Level::Warning:  return t.warning;
      case Level::Error:    return t.error;
      case Level::Debug:    return t.debug;
    }
    return t.info;
  }
  
}