#include <iostream>

#include "cli/types.hpp"

namespace ck::mount {
  void mount(const ck::cli::Context&, const ck::cli::MountArgs&) {
    std::cout << "Hello from mount \n";
  }
}