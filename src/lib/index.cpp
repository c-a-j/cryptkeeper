#include <string>
#include <nlohmann/json.hpp>

#include "util/logger.hpp"
#include "lib/index.hpp"

namespace ck::lib::index { 
  using namespace ck::util::logger;
  void insert(std::string path, std::string key_fpr) {
    logger.info("adding secret: ", path);
  };
}