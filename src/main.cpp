#include <CLI/CLI.hpp>
#include <string>

#include "cli/cli.hpp"
#include "cli/types.hpp"
#include "cmd/run_command.hpp"
#include "error.hpp"
#include "ui/console/console.hpp"

using ck::ui::console::console;

#ifdef __has_feature
#ifdef address_sanitizer
extern "C" const char *__asan_default_options() {
    return "abort_on_error=1:print_legend=0:print_full_thread_history=0:print_"
           "summary=0:verbosity=0:symbolize=0:symbolize_inline_frames=0";
}
#elifdef __SANITIZE_ADDRESS__
extern "C" const char *__asan_default_options() {
    return "abort_on_error=1:print_legend=0:print_full_thread_history=0:print_"
           "summary=0:verbosity=0:symbolize=0:symbolize_inline_frames=0";
}
#endif
#endif

int run_cli(int argc, char **argv) {
    CLI::App app{"crypt-keeper"};

#if CK_ENABLE_DEBUG_LOGS
    ck::ui::console::console.set_debug(true);
#else
    ck::ui::console::console.set_debug(false);
#endif

    try {
        ck::cli::CliArgs args = ck::cli::parse_cli(app, argc, argv);

        if (args.root.no_color) {
            console.set_no_color();
        }

        ck::cmd::run_command(args);
        return 0;
    } catch (const CLI::Success &e) {
        return app.exit(e);
    } catch (const CLI::ParseError &e) {
        console.error(e.what());
        return 1;
    } catch (const ck::error::AppError &e) {
        console.error(e.msg1(), e.msg2());
        return 1;
    } catch (const std::exception &e) {
        console.error(e.what());
        return 1;
    }
    return 0;
}

int main(int argc, char **argv) {
    try {
        console.initialize();
        return run_cli(argc, argv);
    } catch (...) {
        std::fputs("ck: unexpected failure\n", stderr);
    }
}
