#pragma once
#include <CLI/CLI.hpp>
#include <string>

namespace ck::cli {
  struct Vault {
    std::string name;
    std::string key_fpr;
  };
  
  struct Secret {
    std::string path;
    std::string key_fpr;
  };
  
  struct Config {};
  
  void build_cli(CLI::App&, Vault&, Secret&, Config&, int&);
}