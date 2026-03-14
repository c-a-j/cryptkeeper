#include <CLI/CLI.hpp>
#include <string>
#include <iostream>

#include "cli/cli.hpp"
#include "util/logger.hpp"
#include "util/error.hpp"
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
  ck::config::Vault vault = {};
  ck::secret::Secret secret = {};
  ck::config::Config cfg = {};
  std::vector<std::string> set_args;

  CLI::App app{"crypt-keeper"};
  ck::cli::build_cli(app);
  ck::cli::build_config(app, cfg, vault, set_args);
  ck::cli::build_init(app, cfg, vault);
  ck::cli::build_insert(app, cfg, vault, secret);
  ck::cli::build_show(app, cfg, vault, secret);
  
  
  if (argc == 1) {
    std::cout << app.help() << '\n';
    return 0;
  }
  
  try {
    app.parse(argc,argv);
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
