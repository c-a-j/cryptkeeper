#include <iostream>

#include "utils/logger.hpp"
#include "global.hpp"

using namespace ck::logger;

namespace {
  const char* ansi(Color c) {
    switch (c) {
      case Color::Gray:   return "\033[38;2;146;131;116m";
      case Color::Red:    return "\033[38;2;251;73;52m";
      case Color::Green:  return "\033[38;2;184;187;38m";
      case Color::Yellow: return "\033[38;2;250;189;47m";
      case Color::Blue:   return "\033[38;2;131;165;152m";
      case Color::Aqua:   return "\033[38;2;142;192;124m";
      case Color::Orange: return "\033[38;2;254;128;25m";
      case Color::Purple: return "\033[38;2;211;134;155m";
      case Color::Default: return "";
    }
    return "";
  }
  
  Color get_level_color(Level level) {
    switch (level) {
      case Level::Info:     return Color::Gray;
      case Level::Success:  return Color::Green;
      case Level::Warning:  return Color::Yellow;
      case Level::Error:    return Color::Red;
      case Level::Debug:    return Color::Yellow;
    }
    return Color::Gray;
  }
  
  Theme gen_theme() {
    Theme t{};
      t.info    = { get_level_color(Level::Info), "[.]", Stream::Out };
      t.success = { get_level_color(Level::Success), "[*]", Stream::Out };
      t.warning = { get_level_color(Level::Warning), "[W]", Stream::Out };
      t.error   = { get_level_color(Level::Error), "[E]", Stream::Err };
      t.debug   = { get_level_color(Level::Debug), "[D]", Stream::Out };
      t.prefix  = Color::Purple;
      return t;
  };
  
  Theme& default_theme() {
    static Theme t = gen_theme();
    return t;
  }
  
  const ThemeEntry& theme_entry(const Theme& t, Level l) {
    switch (l) {
      case Level::Info: return t.info;
      case Level::Success: return t.success;
      case Level::Warning: return t.warning;
      case Level::Error: return t.error;
      case Level::Debug: return t.debug;
    }
    return t.info;
  }
}

void Logger::emit(Level level, std::string_view msg, Overrides o) {
  const ThemeEntry& e = theme_entry(theme_, level);
  const char* color_code = (o.color == Color::Default) 
    ? ansi(e.color)
    : ansi(o.color);
  
  const char* prefix_color_code = ansi(theme_.prefix);
  
  Stream stream = (o.stream == Stream::Default)
    ? e.stream
    : o.stream;
  
  std::ostream& out = (stream == Stream::Out) 
    ? std::cout 
    : std::cerr;
  
  if (o.prefix) {
    out << prefix_color_code << CLI_ABBR << "  " << theme_.reset
      << color_code << e.tag << theme_.reset << "  ";
  }
  
  out << color_code << msg << theme_.reset;
  if (o.newline) out << "\n";
}

void Logger::emit(Level level, std::string_view msg1, std::string_view msg2) {
  emit(level, msg1, Overrides {}.set_newline(false));
  emit(level, msg2, Overrides {}
    .set_color(get_level_color(Level::Info))
    .set_prefix(false));
}

void Logger::info(std::string_view msg, Overrides o) {
  emit(Level::Info, msg, o);
}
void Logger::info(std::string_view msg1, std::string_view msg2) {
  emit(Level::Info, msg1, msg2);
}

void Logger::success(std::string_view msg, Overrides o) {
  emit(Level::Success, msg, o);
}
void Logger::success(std::string_view msg1, std::string_view msg2) {
  emit(Level::Success, msg1, msg2);
}

void Logger::warning(std::string_view msg, Overrides o) {
  emit(Level::Warning, msg, o);
}
void Logger::warning(std::string_view msg1, std::string_view msg2) {
  emit(Level::Warning, msg1, msg2);
}

void Logger::error(std::string_view msg, Overrides o) {
  emit(Level::Error, msg, o);
}
void Logger::error(std::string_view msg1, std::string_view msg2) {
  emit(Level::Error, msg1, msg2);
}

void Logger::debug(std::string_view msg, Overrides o) {
  emit(Level::Debug, msg, o);
}
void Logger::debug(std::string_view msg1, std::string_view msg2) {
  emit(Level::Debug, msg1, msg2);
}

namespace ck::logger {
  Logger logger(default_theme());
}