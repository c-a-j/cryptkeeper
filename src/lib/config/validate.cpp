#include <expected>

#include "error.hpp"
#include "lib/config/config.hpp"
#include "ui/console/console.hpp"

namespace ck::config {
  using ck::error::Error;
  using ck::error::ConfigErrc;
  using enum ck::error::ConfigErrc;
  using ck::ui::console::console;
  
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
    
    for (const auto& [key, member] : fields()) {
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
