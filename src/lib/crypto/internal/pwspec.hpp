#pragma once

#include <cstddef>

#include "lib/config/pwgen.hpp"

namespace ck::crypto {
struct PwSpec {
    std::size_t length;
    std::size_t symbols;
    std::size_t numbers;
    std::size_t uppercase;
    std::size_t lowercase;
};

PwSpec pwgen_spec(const ck::config::PwGen &pwgen);
} // namespace ck::crypto
