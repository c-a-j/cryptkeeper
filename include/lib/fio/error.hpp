#pragma once

#include <error.hpp>

namespace ck::error {
enum class FsErrc {
    CloseFailed,
    CreateDirectoryFailed,
    FileSystemError,
    FsyncFailed,
    MkstempFailed,
    OpenDirectoryFailed,
    OpenFailed,
    RenameFailed,
    SyncDirectoryFailed,
    WriteFailed,
};
template <>
inline std::string_view Error<FsErrc>::label(FsErrc c) {
    switch (c) {
    case FsErrc::CloseFailed:
        return "Failed to close file";
    case FsErrc::CreateDirectoryFailed:
        return "Failed to create config directory";
    case FsErrc::FileSystemError:
        return "Filesystem error";
    case FsErrc::FsyncFailed:
        return "Fsync failed";
    case FsErrc::MkstempFailed:
        return "mkstemp failed";
    case FsErrc::OpenDirectoryFailed:
        return "Failed to open directory";
    case FsErrc::OpenFailed:
        return "Failed to open file";
    case FsErrc::RenameFailed:
        return "Failed to rename file";
    case FsErrc::SyncDirectoryFailed:
        return "Failed to sync directory";
    case FsErrc::WriteFailed:
        return "Failed to write to file";
    default:
        return "Unknown error";
    }
}
} // namespace ck::error
