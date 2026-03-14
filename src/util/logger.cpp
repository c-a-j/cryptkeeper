#include <iostream>

#include "util/logger.hpp"
#include "util/term.hpp"
#include "global.hpp"

namespace {
  using namespace ck::util::logger;
  using namespace ck::util::term;
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
  }
  
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

namespace ck::util::logger {
using namespace ck::util::term;

  void Logger::emit(Level level, std::string_view msg, Overrides o) {
    const ThemeEntry& e = theme_entry(theme_, level);
    std::string color_code = (o.color == Color::Default) 
      ? ansi(e.color)
      : ansi(o.color);
    
    std::string prefix_color_code = ansi(theme_.prefix);
    
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
 
  // This logger function is for printing messages with two colors so only the
  // most important part (the first part) is colorful.
  void Logger::emit(Level level, std::string_view msg1, std::string_view msg2) {
    emit(level, std::string(msg1) + ": ", Overrides {}.set_newline(false));
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
  
  Logger logger(default_theme());
}