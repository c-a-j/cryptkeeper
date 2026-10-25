#include "cli/types.hpp"
#include "ui/console/console.hpp"

namespace ck::cmd {
using ui::console::console;

void replace(const ck::cli::Context &_, const ck::cli::ReplaceArgs &args) {
    std::string path;
    if (args.path) {
        path = *args.path;
    }
    console.debug("Replacing secret '" + path + "'");
}
} // namespace ck::cmd
