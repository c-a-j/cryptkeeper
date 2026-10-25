#include <expected>

#include "error.hpp"
#include "lib/config/config.hpp"
#include "ui/console/console.hpp"

namespace ck::config {
using ck::error::ConfigErrc;
using ck::error::Error;
using enum ck::error::ConfigErrc;
using ck::ui::console::console;

std::expected<void, Error<ConfigErrc>> State::valid_pwgen() const {
    return this->pwgen.valid();
}

std::expected<void, Error<ConfigErrc>> Pwgen::valid() const {
    auto fail = [](const std::string &msg) {
        auto err = Error<ConfigErrc>{InvalidConfigValue, msg};
        return std::unexpected(err);
    };

    if (length > 100) {
        return fail("maximum pwgen password length is 100");
    }

    if (length < 1) {
        return fail("minimum pwgen password length is 1");
    }

    if (numbers < 0) {
        return fail("values must be >= 0");
    }

    if (symbols < 0) {
        return fail("values must be >= 0");
    }

    if (uppercase < 0) {
        return fail("values must be >= 0");
    }

    if (lowercase < 0) {
        return fail("values must be >= 0");
    }

    int sum = symbols + numbers + uppercase + lowercase;

    if (length < sum) {
        return fail("password length must be longer than the sum of its parts");
    }

    return {};
}
} // namespace ck::config
