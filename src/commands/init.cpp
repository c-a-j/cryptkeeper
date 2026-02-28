#include <string>
#include <filesystem>
#include <sstream>

#include "commands/init.hpp"
#include "global.hpp"
#include "utils/logger.hpp"

namespace fs = std::filesystem;
using ck::logger::logger;

static std::string env_or_empty(const char* name) {
  if (const char* value = std::getenv(name)) return value;
  return {};
}

fs::path crypt_root() {
#ifdef _WIN32
  auto appdata = env_or_empty("APPDATA");
  if (appdata.empty()) throw std::runtime_error("APPDATA is not set");
  return fs::path(appdata) / APP_NAME;
#else
  auto xdg_data = env_or_empty("XDG_DATA_HOME");
  if (!xdg_data.empty()) return fs::path(xdg_data);
  
  auto home = env_or_empty("HOME");
  if (home.empty()) throw std::runtime_error("HOME is not set");
  return fs::path(home) / ".local" / "share" / APP_NAME;
#endif
}

void init_crypt(const char* crypt_name) {
  fs::path dir = crypt_root() / crypt_name;
  std::error_code ec;
  bool created = fs::create_directories(dir, ec);
  if (ec) {
    logger.error("Failed to create config dir: ", ec.message());
    return;
  }
  
  if (!created) {
    std::ostringstream ss;
    logger.error("Crypt already exists: ", dir.string());
  } else {
    logger.success("Crypt initialized: ", dir.string());
  }
}

void run_init() {
  init_crypt("foo");
}