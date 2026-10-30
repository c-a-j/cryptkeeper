#pragma once
#include <string_view>

#include <error.hpp>

namespace ck::error {
enum class ConfigErrc {
    CreateDirectoryFailed,
    CreateConfigFailed,
    WriteConfigFailed,
    AlreadyExists,
    DoesNotExist,
    InvalidSetParameter,
    InvalidConfigFile,
    InvalidConfigKey,
    InvalidConfigValue,
    IoError,
};
template <>
inline std::string_view Error<ConfigErrc>::label(ConfigErrc c) {
    switch (c) {
    case ConfigErrc::CreateDirectoryFailed:
        return "Failed to create crypt";
    case ConfigErrc::CreateConfigFailed:
        return "Failed to create conifig file";
    case ConfigErrc::WriteConfigFailed:
        return "Failed to write conifig file";
    case ConfigErrc::AlreadyExists:
        return "Configuration file already exists";
    case ConfigErrc::DoesNotExist:
        return "Configuration file not found";
    case ConfigErrc::InvalidSetParameter:
        return "Invalid configuration scope";
    case ConfigErrc::InvalidConfigFile:
        return "Invalid configuration file";
    case ConfigErrc::InvalidConfigKey:
        return "Invalid configuration key";
    case ConfigErrc::InvalidConfigValue:
        return "Invalid configuration value";
    case ConfigErrc::IoError:
        return "I/O error";
    default:
        return "Unknown error";
    }
}
} // namespace ck::error
