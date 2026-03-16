#include <iostream>

#include "util/logger/logger.hpp"
#include "util/logger/theme.hpp"
#include "util/term.hpp"
#include "global.hpp"

namespace ck::util::logger {
using namespace ck::util::term;
  void Logger::emit(Level level, std::string_view msg, Overrides o) {
    const LevelStyle& s = style_for(theme_, level);
    std::string color_code = (!o.style) 
      ? ansi(s.colors.primary)
      : ansi(*o.style);
    
    std::string prefix_color_code = ansi(s.colors.prefix);
    
    Stream stream = o.stream.value_or(s.stream);
    
    std::ostream& out = (stream == Stream::Out) 
      ? std::cout 
      : std::cerr;
    
    if (o.prefix) {
      out << prefix_color_code << CLI_ABBR << "  " << reset()
        << color_code << s.tag << reset() << "  ";
    }
    
    out << color_code << msg << reset();
    if (o.newline) out << "\n";
  }
 
  // This logger function is for printing messages with two colors so only the
  // most important part (the first part) is colorful.
  void Logger::emit(Level level, std::string_view msg1, std::string_view msg2) {
    const LevelStyle& s = style_for(theme_, level);
    emit(level, std::string(msg1) + ": ", Overrides {}.set_newline(false));
    emit(level, msg2, Overrides {}
      .set_color(s.colors.secondary)
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