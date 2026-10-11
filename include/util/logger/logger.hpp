#pragma once
#include <string_view>
#include <utility>
#include <optional>

#include "util/logger/theme.hpp"

namespace  ck::util::logger {
  struct LoggerConfig {
    bool no_color = false;
    bool debug = true;
  };
  
  class Logger {
    public:
      Logger() noexcept = default;
      void initialize(Theme theme);
      void initialize();

      void emit(Level lvl, std::string_view msg, Overrides o = {});
      void emit(Level lvl, std::string_view msg1, std::string_view msg2);

      void info(std::string_view msg, Overrides o = {});
      void info(std::string_view msg1, std::string_view msg2);

      void success(std::string_view msg, Overrides o = {});
      void success(std::string_view msg1, std::string_view msg2);

      void warning(std::string_view msg, Overrides o = {});
      void warning(std::string_view msg1, std::string_view msg2);

      void error(std::string_view msg, Overrides o = {});
      void error(std::string_view msg1, std::string_view msg2);

      void debug(std::string_view msg, Overrides o = {});
      void debug(std::string_view msg1, std::string_view msg2);
      
      void configure(const LoggerConfig& cfg);
      
      void set_no_color();
      
      void set_debug(bool debug);
      
    private:
      std::optional<Theme> theme_;
      LoggerConfig cfg_;
  };

  extern Logger logger;
}
