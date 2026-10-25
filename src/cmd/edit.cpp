#include "cli/types.hpp"
#include "ui/console/console.hpp"

namespace ck::cmd {
using ui::console::console;

void edit(const ck::cli::Context &_, const ck::cli::EditArgs &args) {
    std::string path;
    if (args.path) {
        path = *args.path;
    }
    console.debug("Editing secret '" + path + "'");
}
} // namespace ck::cmd
