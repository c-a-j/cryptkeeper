#pragma once
#include <string_view>

#include "util/logger/theme.hpp"

namespace  ck::util::logger {
  struct LoggerConfig {
    bool no_color = false;
    bool debug = true;
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
      
      void configure(const LoggerConfig& cfg);
      
      void set_no_color();
      
      void set_debug(const bool);
      
    private:
      Theme theme_;
      LoggerConfig cfg_;
  };

  extern Logger logger;
}
