#pragma once

#include <error.hpp>

namespace ck::error {
enum class PathErrc {
    CreateDirectoryFailed,
    DataHomeNotSet,
    DirectoryDoesNotExist,
    DoesNotExist,
    FileDoesNotExist,
    FileSystemError,
    HomeNotSet,
    LocalAppDataNotSet,
    CryptUnspecified,
};

template <>
inline std::string_view Error<PathErrc>::label(PathErrc c) {
    switch (c) {
    case PathErrc::CreateDirectoryFailed:
        return "Failed to create config directory";
    case PathErrc::DataHomeNotSet:
        return "XDG_DATA_HOME not set";
    case PathErrc::DirectoryDoesNotExist:
        return "Directory does not exist";
    case PathErrc::DoesNotExist:
        return "Directory or file does not exist";
    case PathErrc::FileDoesNotExist:
        return "File does not exist";
    case PathErrc::FileSystemError:
        return "Filesystem error";
    case PathErrc::HomeNotSet:
        return "HOME not set";
    case PathErrc::LocalAppDataNotSet:
        return "LOCALAPPDATA not set";
    case PathErrc::CryptUnspecified:
        return "Crypt unspecified";
    default:
        return "Unknown error";
    }
}
} // namespace ck::error
