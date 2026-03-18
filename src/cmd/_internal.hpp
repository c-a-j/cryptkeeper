#pragma once

#include "cli/types.hpp"

namespace ck::cmd {
  void config(const ck::cli::Context&, const ck::cli::ConfigArgs&);
  void init(const ck::cli::Context&, const ck::cli::InitArgs&);
  void insert(const ck::cli::Context&, const ck::cli::InsertArgs&);
  void show(const ck::cli::Context&, const ck::cli::ShowArgs&);
  void mount(const ck::cli::Context&, const ck::cli::MountArgs&);
  void umount(const ck::cli::Context&, const ck::cli::UmountArgs&);
}