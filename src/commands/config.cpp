#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <cstdlib>

#include "commands/config.hpp"
#include "global.hpp"
#include "utils/logger.hpp"

namespace fs = std::filesystem;

static std::string env_or_empty(const char* name) {
  if (const char* value = std::getenv(name)) return value;
  return {};
}

fs::path config_root() {
#ifdef _WIN32
  auto appdata = env_or_empty("APPDATA");
  if (appdata.empty()) throw std::runtime_error("APPDATA is not set");
  return fs::path(appdata);
#else
  auto xdg = env_or_empty("XDG_CONFIG_HOME");
  if (!xdg.empty()) return fs::path(xdg);
  
  auto home = env_or_empty("HOME");
  if (home.empty()) throw std::runtime_error("HOME is not set");
  return fs::path(home) / ".config";
#endif
}

fs::path app_config_dir() {
  return config_root() / APP_NAME;
}

void create_config_dir() {
  fs::path dir = app_config_dir();
  std::error_code ec;
  std::filesystem::create_directories(dir, ec);
  if (ec) {
    std::cerr << 
      "Failed to create config dir: " << dir << "\n" << 
      ec.message() << "\n";
  }
}

std::string print_defaults() {
  std::ostringstream ss;
  ss << CONFIG_HEADER << "\n"
    << "default-store = \"valult\"" << "\n"
    << "auto-push = false" << "\n";
  return ss.str();
}

void create_config_file() {
  fs::path dir = app_config_dir();
  fs::path cfg = dir / "config.toml";
  if (fs::exists(cfg)) return;
  
  std::ofstream out(cfg, std::ios::out | std::ios::trunc);
  out << print_defaults() ; 
  if (!out) {
    std::cerr << "Failed to create config file: " << cfg << "\n";
    return;
  }
  out.close();
}


void run_config() {
  std::cout << "Looking for a configuration in " << app_config_dir() << "\n";
  create_config_dir();
  create_config_file();
}