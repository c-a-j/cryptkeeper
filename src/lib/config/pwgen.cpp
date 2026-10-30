#include "error.hpp"
#include "lib/config/config.hpp"
#include "ui/console/console.hpp"

namespace ck::config {
using ck::error::ConfigErrc;
using ck::error::Error;
using enum ck::error::ConfigErrc;
using ck::ui::console::console;

std::expected<void, Error<ConfigErrc>> validate_pwgen(const PwGen &pwgen) {
    auto fail = [](const std::string &msg) {
        auto err = Error<ConfigErrc>{InvalidConfigValue, msg};
        return std::unexpected(err);
    };

    if (pwgen.length < 1 || pwgen.length > 100) {
        return fail("pwgen password length must be between 1 and 100");
    }

    if (pwgen.numbers < 0 || pwgen.numbers > 100) {
        return fail("values must be between 0 and 100");
    }

    if (pwgen.symbols < 0 || pwgen.symbols > 100) {
        return fail("values must be between 0 and 100");
    }

    if (pwgen.uppercase < 0 || pwgen.uppercase > 100) {
        return fail("values must be between 0 and 100");
    }

    if (pwgen.lowercase < 0 || pwgen.lowercase > 100) {
        return fail("values must be between 0 and 100");
    }

    int sum = pwgen.symbols + pwgen.numbers + pwgen.uppercase + pwgen.lowercase;

    if (pwgen.length < sum) {
        return fail("password length cannot be shorter than the sum of its parts");
    }

    return {};
}
} // namespace ck::config
