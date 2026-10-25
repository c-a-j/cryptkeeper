#pragma once
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

#include <error.hpp>

namespace ck::error {
enum class InitErrc {
    KeyNotFound,
    CreateDirectoryFailed,
    OpenGpgIdFailed,
    WriteGpgIdFailed,
    OpenIndexFailed,
    AlreadyExists,
    IoError,
    NoPathToIndex,
    WriteIndexFailed
};
template <> inline std::string_view Error<InitErrc>::label(InitErrc c) {
    switch (c) {
    case InitErrc::KeyNotFound:
        return "Public key not found";
    case InitErrc::CreateDirectoryFailed:
        return "Failed to create crypt";
    case InitErrc::OpenGpgIdFailed:
        return "Failed to open .gpg-id";
    case InitErrc::WriteGpgIdFailed:
        return "Failed to write .gpg-id";
    case InitErrc::AlreadyExists:
        return "Crypt already exists";
    case InitErrc::IoError:
        return "I/O error";
    case InitErrc::OpenIndexFailed:
        return "Failed to open index file";
    case InitErrc::WriteIndexFailed:
        return "Failed to create a new index file";
    default:
        return "Unknown error";
    }
}
} // namespace ck::error
