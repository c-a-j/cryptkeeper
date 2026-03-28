#pragma once

#include <cstddef>
#include <string>
#include <filesystem>
#include <span>

#ifdef _WIN32
  #include <windows.h>
#endif

namespace ck::fio {
  namespace fs = std::filesystem;
  
  struct TempFile {
    fs::path path;
    fs::path target;
    bool active = true;
#ifdef _WIN32
    HANDLE handle = INVALID_HANDLE_VALUE;
#else
    int fd = -1;
#endif
    
    explicit TempFile(const fs::path&);

    TempFile(const TempFile&) = delete;
    TempFile& operator=(const TempFile&) = delete;
    
    int write(std::span<const std::byte> bytes) noexcept;
    int write(const std::string& contents) noexcept;

    int sync() noexcept;

    int close() noexcept;
    
    int commit() noexcept;

    ~TempFile() noexcept;
  };
}
