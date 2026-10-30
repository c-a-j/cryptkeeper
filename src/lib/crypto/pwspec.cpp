#include "./internal/pwspec.hpp"

#include "lib/config/pwgen.hpp"

namespace ck::crypto {
PwSpec pwgen_spec(const ck::config::PwGen &pwgen) {
    PwSpec spec = {
        .length = static_cast<std::size_t>(pwgen.length),
        .symbols = static_cast<std::size_t>(pwgen.symbols),
        .numbers = static_cast<std::size_t>(pwgen.numbers),
        .uppercase = static_cast<std::size_t>(pwgen.uppercase),
        .lowercase = static_cast<std::size_t>(pwgen.lowercase)};

    return spec;
}
} // namespace ck::crypto
