#include "error.hpp"
#include "lib/config/config.hpp"
#include "ui/console/console.hpp"

namespace ck::config {
  using ck::error::Error;
  using ck::error::ConfigErrc;
  using enum ck::error::ConfigErrc;
  using ck::ui::console::console;
  
  ck::crypto::PwSpec Pwgen::spec() const {
    ck::crypto::PwSpec spec = {
      .length = static_cast<std::size_t>(this->length),
      .symbols = static_cast<std::size_t>(this->symbols),
      .numbers = static_cast<std::size_t>(this->numbers),
      .uppercase = static_cast<std::size_t>(this->uppercase),
      .lowercase = static_cast<std::size_t>(this->lowercase)
    };

    return spec;
  }
}
