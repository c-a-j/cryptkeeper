#include <expected>

#include "util/error.hpp"
#include "lib/config/types.hpp"
#include "util/logger/logger.hpp"

namespace ck::config {
  using ck::util::error::Error;
  using ck::util::error::ConfigErrc;
  using enum ck::util::error::ConfigErrc;
  using ck::util::logger::logger;
  
  std::expected<void, Error<ConfigErrc>> State::valid_pwgen() {
    return this->pwgen.valid();
  }

  std::expected<void, Error<ConfigErrc>> Pwgen::valid() {
    auto fail = [](const std::string& msg) {
      auto err = Error<ConfigErrc>{InvalidConfigValue, msg};
      return std::unexpected(err);
    };
    
    if (length > 100) {
      return fail("maximum pwgen password length is 100");
    }
    
    for (auto& [key, member] : fields()) {
      auto result = std::visit([&](auto m) -> std::expected<void, Error<ConfigErrc>> {
        if (this->*m < 0) {
          return fail("values must be >= 0");
        }
        return {};
      }, member);

      if (!result) {
        return result;
      }
    }
    
    int sum = symbols + numbers + uppercase + lowercase ;
    
    
    if (length < sum) {
      return fail("password length must be longer than the sum of its parts");
    }

    return {};
  }
}
