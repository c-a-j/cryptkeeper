#pragma once

#include "cli/types.hpp"

namespace ck::cmd {
  void config(const ck::cli::Context& _, const ck::cli::ConfigArgs& args);
  void init(const ck::cli::Context& _, const ck::cli::InitArgs& args);
  void insert(const ck::cli::Context& _, const ck::cli::InsertArgs& args);
  void show(const ck::cli::Context& _, const ck::cli::ShowArgs& args);
  void mount(const ck::cli::Context& _, const ck::cli::MountArgs& args);
  void umount(const ck::cli::Context& _, const ck::cli::UmountArgs& args);
  void chroot(const ck::cli::Context& _, const ck::cli::ChrootArgs& args);
}
