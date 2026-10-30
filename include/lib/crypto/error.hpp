#pragma once

#include <error.hpp>

namespace ck::error {
enum class CryptoErrc {
    FillRandomBytesFailed,
    GpgmeFailed,
    GpgmeDataNewFailed,
    GpgmeDataNewFromMemFailed,
    GpgmeDataReleaseFailed,
    GpgmeDataReleaseAndGetMemFailed,
    GpgmeNewFailed,
    GpgmeNewFromMemFailed,
    GpgmeGetKeyFailed,
    GpgmeSetProtocolFailed,
    GpgmeOpGenKeyFailed,
    GpgmeOpGenKeyResultFailed,
    InvalidPwSpec,
    GpgKeyNotFound,
    InvalidSecureBytesArgs,
    FailedToReadFile,
    FailedToOpenFile,
};
template <>
inline std::string_view Error<CryptoErrc>::label(CryptoErrc c) {
    switch (c) {
    case CryptoErrc::FillRandomBytesFailed:
        return "Fill random bytes failure";
    case CryptoErrc::GpgmeFailed:
        return "gpgme failure";
    case CryptoErrc::GpgmeDataNewFailed:
        return "gpgme_data_new failure";
    case CryptoErrc::GpgmeDataNewFromMemFailed:
        return "gpgme_data_new_from_mem failure";
    case CryptoErrc::GpgmeDataReleaseFailed:
        return "gpgme_data_release failure";
    case CryptoErrc::GpgmeDataReleaseAndGetMemFailed:
        return "gpgme_data_release_and_get_mem failure";
    case CryptoErrc::GpgmeNewFailed:
        return "gpgme_new failure";
    case CryptoErrc::GpgmeNewFromMemFailed:
        return "gpgme_new_from_mem failure";
    case CryptoErrc::GpgmeGetKeyFailed:
        return "gpgme_get_key failure";
    case CryptoErrc::GpgmeSetProtocolFailed:
        return "gpgme_set_protocol failure";
    case CryptoErrc::GpgmeOpGenKeyFailed:
        return "gpgme_op_genkey failure";
    case CryptoErrc::GpgmeOpGenKeyResultFailed:
        return "gpgme_op_genkey_result failure";
    case CryptoErrc::GpgKeyNotFound:
        return "gpg key not found";
    case CryptoErrc::InvalidPwSpec:
        return "invalid password specification";
    case CryptoErrc::InvalidSecureBytesArgs:
        return "Invalid SecureBytes.assign() arguments";
    case CryptoErrc::FailedToReadFile:
        return "Failed to read file";
    case CryptoErrc::FailedToOpenFile:
        return "Failed to Open file";
    default:
        return "Unknown error";
    }
}
} // namespace ck::error
