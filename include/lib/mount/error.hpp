#pragma once

#include <error.hpp>

namespace ck::error {
enum class MountErrc {
    MountFileNotFound,
    WriteMountFailed,
    CryptNotInitialized,
    NoAlias,
    AliasExists,
    AliasDoesNotExist,
    NoRoot,
    InvalidArguments,
    InvalidMountFile,
    SecretAlongAlias,
    ChrootConflict,
};
template <>
inline std::string_view Error<MountErrc>::label(MountErrc c) {
    switch (c) {
    case MountErrc::MountFileNotFound:
        return "Mount file not found";
    case MountErrc::WriteMountFailed:
        return "Failed to write mount file";
    case MountErrc::CryptNotInitialized:
        return "Crypt is not initialized";
    case MountErrc::NoAlias:
        return "Invalid mount file, every mount needs an alias";
    case MountErrc::AliasExists:
        return "Alias already exists";
    case MountErrc::AliasDoesNotExist:
        return "Alias does not exist";
    case MountErrc::NoRoot:
        return "There is no root crypt";
    case MountErrc::InvalidArguments:
        return "Invalid arguments";
    case MountErrc::InvalidMountFile:
        return "Invalid mount file";
    case MountErrc::SecretAlongAlias:
        return "There is a secret along this alias path";
    case MountErrc::ChrootConflict:
        return "Chroot path conflict";
    default:
        return "Unknown error";
    }
}
} // namespace ck::error
