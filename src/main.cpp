#include <CLI/CLI.hpp>
#include <string>

#include "cmd/run_command.hpp"
#include "cli/cli.hpp"
#include "cli/types.hpp"
#include "util/logger/logger.hpp"
#include "util/error.hpp"

#if defined(__has_feature)
#  if __has_feature(address_sanitizer)
extern "C" const char* __asan_default_options() {
  return "abort_on_error=1:print_legend=0:print_full_thread_history=0:print_summary=0:verbosity=0:symbolize=0:symbolize_inline_frames=0";
}
#  endif
#elif defined(__SANITIZE_ADDRESS__)
extern "C" const char* __asan_default_options() {
  return "abort_on_error=1:print_legend=0:print_full_thread_history=0:print_summary=0:verbosity=0:symbolize=0:symbolize_inline_frames=0";
}
#endif

int main(int argc, char** argv) {
  CLI::App app{"crypt-keeper"};
  ck::cli::CliArgs args = ck::cli::parse_cli(app, argc, argv);
  
  ck::util::logger::logger.set_debug(true);
  
  if (args.root.no_color) {
    ck::util::logger::logger.set_no_color();
  }
  
  try {
    ck::cmd::run_command(args);
    return 0;
  } catch (const CLI::Success& e) {
    return app.exit(e);
  } catch (const CLI::ParseError& e) {
    ck::util::logger::logger.error(e.what());
    return app.exit(e);
  } catch (const ck::util::error::AppError& e) {
    ck::util::logger::logger.error(e.msg1, e.msg2);
    return 1;
  }
  return 0;
}
