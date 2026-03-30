#include <expected>

#include "util/error.hpp"
#include "lib/config/types.hpp"
#include "util/logger/logger.hpp"

namespace ck::config {
  using ck::util::error::Error;
  using ck::util::error::ConfigErrc;
  using enum ck::util::error::ConfigErrc;
  using ck::util::logger::logger;
  
  ck::crypto::PwSpec Pwgen::spec() {
    ck::crypto::PwSpec spec = {
      .length = static_cast<std::size_t>(this->length),
      .symbols = static_cast<std::size_t>(this->symbols),
      .numbers = static_cast<std::size_t>(this->uppercase),
      .uppercase = static_cast<std::size_t>(this->uppercase),
      .lowercase = static_cast<std::size_t>(this->lowercase)
    };

    return spec;
  }
}
