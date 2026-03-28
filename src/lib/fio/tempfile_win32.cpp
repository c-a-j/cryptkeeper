#ifdef _WIN32

#include <string>
#include <filesystem>
#include <limits>
#include <vector>
#include <windows.h>

#include "util/error.hpp"
#include "./msg.hpp"
#include "./tempfile.hpp"

namespace ck::fio {
  namespace fs = std::filesystem;
  using ck::util::error::Error;
  using ck::util::error::FsErrc;
  using enum ck::util::error::FsErrc;
  
  int last_error() noexcept {
    return static_cast<int>(::GetLastError());
  }
  
  TempFile::TempFile(const fs::path& target_) : target(target_) {
    fs::path parent = target.parent_path();
    fs::path dir = parent.empty() ? fs::path(".") : parent;

    std::vector<wchar_t> buf(MAX_PATH);
    DWORD result = ::GetTempFileNameW(dir.c_str(), L"ckp", 0, buf.data());
    if (result == 0) {
      throw Error<FsErrc>{MkstempFailed, msg(target, last_error())};
    }

    try {
      path = buf.data();
    } catch (const std::exception& e) {
      ::DeleteFileW(buf.data());
      throw Error<FsErrc>{FileSystemError, e.what()};
    }

    handle = ::CreateFileW(
      path.c_str(),
      GENERIC_WRITE,
      0,
      nullptr,
      TRUNCATE_EXISTING,
      FILE_ATTRIBUTE_NORMAL,
      nullptr
    );
    if (handle == INVALID_HANDLE_VALUE) {
      int err = last_error();
      ::DeleteFileW(path.c_str());
      throw Error<FsErrc>{OpenFailed, msg(path, err)};
    }
  }

  int TempFile::write(std::span<const std::byte> bytes) noexcept {
    const std::byte* data = bytes.data();
    std::size_t remaining = bytes.size();

    while (remaining > 0) {
      DWORD chunk = remaining > static_cast<std::size_t>(std::numeric_limits<DWORD>::max())
        ? std::numeric_limits<DWORD>::max()
        : static_cast<DWORD>(remaining);
      DWORD written = 0;
      if (!::WriteFile(handle, data, chunk, &written, nullptr)) {
        return last_error();
      }
      if (written == 0) {
        return ERROR_WRITE_FAULT;
      }
      std::size_t count = static_cast<std::size_t>(written);
      data += count;
      remaining -= count;
    }

    return 0;
  }

  int TempFile::write(const std::string& contents) noexcept {
    std::span<const char> chars(contents.data(), contents.size());
    return write(std::as_bytes(chars));
  }

  int TempFile::sync() noexcept {
    if (!::FlushFileBuffers(handle)) {
      return last_error();
    }
    return 0;
  }

  int TempFile::close() noexcept {
    if (handle == INVALID_HANDLE_VALUE) {
      return 0;
    }

    if (!::CloseHandle(handle)) {
      int err = last_error();
      handle = INVALID_HANDLE_VALUE;
      return err;
    }

    handle = INVALID_HANDLE_VALUE;
    return 0;
  }

  int TempFile::commit() noexcept {
    if (!::MoveFileExW(
      path.c_str(),
      target.c_str(),
      MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH
    )) {
      return last_error();
    }
    active = false;
    return 0;
  }

  TempFile::~TempFile() noexcept {
    close();
    if (active) {
      ::DeleteFileW(path.c_str());
    }
  }
}

#endif
