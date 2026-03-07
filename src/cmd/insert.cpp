#include "cmd/insert.hpp"
#include "util/logger.hpp"
#include "lib/index.hpp"

namespace ck::cmd::insert {
  using namespace ck::util::logger;
  void insert_secret(std::string secret_path, std::string key_fpr) {
    ck::lib::index::insert(secret_path, key_fpr);
  }
}