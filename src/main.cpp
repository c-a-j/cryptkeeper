#include <CLI/CLI.hpp>
#include <string>
#include <iostream>

#include "cli/build_cli.hpp"
#include "util/logger.hpp"
#include "lib/types.hpp"

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
  ck::types::Vault vault = {};
  ck::types::Secret secret = {};
  ck::types::Config cfg = {};
  int exit_code = 0;

  CLI::App app{"crypt-keeper"};
  ck::cli::build_cli(app, vault, secret, cfg, exit_code);
  if (argc == 1) {
    std::cout << app.help() << '\n';
    return 0;
  }
  
  try {
    app.parse(argc,argv);
    return exit_code;
  } catch (const CLI::Success& e) {
    return app.exit(e);
  } catch (const CLI::ParseError& e) {
    ck::util::logger::logger.error(e.what());
  }
  return 0;
}
