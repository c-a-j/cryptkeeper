#include <string>
#include <filesystem>

#include "util/error.hpp"
#include "./msg.hpp"
#include "./tempfile.hpp"

namespace ck::fs {
  namespace fs = std::filesystem;
  using ck::util::error::Error;
  using ck::util::error::PathErrc;
  using enum ck::util::error::PathErrc;

  void write_atomic(const fs::path& target, const std::string& contents) {
    std::error_code ec;
    fs::path parent = target.parent_path();
    if (!parent.empty()) {
      fs::create_directories(parent, ec);
    }
    if (ec) {
      throw Error<PathErrc>{CreateDirectoryFailed, parent.string() + ": " + ec.message()};
    }

    TempFile tmp(target);
    int err = tmp.write(contents);
    if (err) {
      throw Error<PathErrc>{WriteFailed, msg(tmp.path, err)};
    }

    err = tmp.sync();
    if (err) {
      throw Error<PathErrc>{FsyncFailed, msg(tmp.path, err)};
    }

    err = tmp.close();
    if (err) {
      throw Error<PathErrc>{CloseFailed, msg(tmp.path, err)};
    }

    err = tmp.commit();
    if (err) {
      throw Error<PathErrc>{RenameFailed, msg(tmp.path, err) + " -> " + target.string()};
    }
  }
}
