#pragma once
#include <string_view>

#include "util/term.hpp"

namespace  ck::util::logger {
  using namespace ck::util::term;
  enum class Stream { Out, Err, Default };
  
  enum class Level {
    Info,
    Success,
    Warning,
    Error,
    Debug
  };
  
  struct Overrides {
    Color color = Color::Default;
    Stream stream = Stream::Default;
    bool prefix = true;
    bool newline = true;
    
    Overrides& set_color(Color v) { color = v; return *this; }
    Overrides& set_stream(Stream v) { stream = v; return *this; }
    Overrides& set_prefix(bool v) { prefix = v; return *this; }
    Overrides& set_newline(bool v) { newline = v; return *this; }
  };
  
  struct ThemeEntry {
    Color color;
    const char* tag;
    Stream stream;
  };
  
  struct Theme {
    ThemeEntry info, success, warning, error, debug;
    Color prefix;
    const char* reset = "\033[0m";
  };
  
  class Logger {
    public:
      explicit Logger(Theme theme) : theme_(theme) {}
      void emit(Level, std::string_view, Overrides = {});
      void emit(Level, std::string_view, std::string_view);

      void info(std::string_view, Overrides = {});
      void info(std::string_view, std::string_view);

      void success(std::string_view, Overrides = {});
      void success(std::string_view, std::string_view);

      void warning(std::string_view, Overrides = {});
      void warning(std::string_view, std::string_view);

      void error(std::string_view, Overrides = {});
      void error(std::string_view, std::string_view);

      void debug(std::string_view, Overrides = {});
      void debug(std::string_view, std::string_view);
    private:
      Theme theme_;
  };

  extern Logger logger;
}
