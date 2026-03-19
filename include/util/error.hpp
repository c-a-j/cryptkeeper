#pragma once
#include <stdexcept>

namespace ck::util::error {
  struct AppError : std::runtime_error {
    std::string msg1;
    std::string msg2 = {};
    AppError(std::string m1, std::string m2 = {})
      : std::runtime_error(m1 + m2), msg1(std::move(m1)), msg2(std::move(m2))  {}
  };
  
  template<typename T>
  struct Error : AppError {
    T code;
    
    Error(T c, std::string m2 = {})
      : AppError(std::string(label(c)), std::move(m2)), code(c) {}
      
    private:
      static std::string_view label(T c);
  };
  
  enum class InitErrc {
    KeyNotFound,
    CreateDirectoryFailed,
    OpenGpgIdFailed,
    WriteGpgIdFailed,
    AlreadyExists,
    IoError,
  };
  template<>
  inline std::string_view Error<InitErrc>::label(InitErrc c) {
    switch (c) {
      case InitErrc::KeyNotFound:           return "Public key not found";
      case InitErrc::CreateDirectoryFailed: return "Failed to create vault";
      case InitErrc::OpenGpgIdFailed:       return "Failed to open .gpg-id";
      case InitErrc::WriteGpgIdFailed:      return "Failed to write .gpg-id";
      case InitErrc::AlreadyExists:         return "Vault already exists";
      case InitErrc::IoError:               return "I/O error";
      default:                              return "Unknown error";
    }
  }
  
  enum class ConfigErrc {
    CreateDirectoryFailed,
    CreateConfigFailed,
    SaveConfigFailed,
    AlreadyExists,
    DoesNotExist,
    InvalidSetParameter,
    InvalidConfigFile,
    InvalidConfigKey,
    IoError,
  };
  template<>
  inline std::string_view Error<ConfigErrc>::label(ConfigErrc c) {
    switch (c) {
      case ConfigErrc::CreateDirectoryFailed: return "Failed to create vault";
      case ConfigErrc::CreateConfigFailed:    return "Failed to create conifig file";
      case ConfigErrc::SaveConfigFailed:      return "Failed to save conifig file";
      case ConfigErrc::AlreadyExists:         return "Configuration file already exists";
      case ConfigErrc::DoesNotExist:          return "Configuration file not found";
      case ConfigErrc::InvalidSetParameter:   return "Invalid configuration scope";
      case ConfigErrc::InvalidConfigFile:     return "Invalid configuration file";
      case ConfigErrc::InvalidConfigKey:      return "Invalid configuration key";
      case ConfigErrc::IoError:               return "I/O error";
      default:                                return "Unknown error";
    }
  }
    
    enum class CryptoErrc {
      FillRandomBytesFailed,
      GpgmeFailed,
      GpgmeNewFailed,
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
    template<>
    inline std::string_view Error<CryptoErrc>::label(CryptoErrc c) {
      switch (c) {
        case CryptoErrc::FillRandomBytesFailed:     return "Fill random bytes failure";
        case CryptoErrc::GpgmeFailed:               return "gpgme failure";
        case CryptoErrc::GpgmeNewFailed:            return "gpgme_new failure";
        case CryptoErrc::GpgmeGetKeyFailed:         return "gpgme_get_key failure";
        case CryptoErrc::GpgmeSetProtocolFailed:    return "gpgme_set_protocol failure";
        case CryptoErrc::GpgmeOpGenKeyFailed:       return "gpgme_op_genkey failure";
        case CryptoErrc::GpgmeOpGenKeyResultFailed: return "gpgme_op_genkey_result failure";
        case CryptoErrc::GpgKeyNotFound:            return "gpg key not found";
        case CryptoErrc::InvalidPwSpec:             return "invalid password specification";
        case CryptoErrc::InvalidSecureBytesArgs:    return "Invalid SecureBytes.assign() arguments";
        case CryptoErrc::FailedToReadFile:          return "Failed to read file";
        case CryptoErrc::FailedToOpenFile:          return "Failed to Open file";
        default:                                    return "Unknown error";
      }
    }
    
    enum class IndexErrc {
      VaultUnspecified,
      NoPath,
      SecretExists,
      PathConflict,
      SecretNotFound,
      UndefinedOptional,
      GpgIdFileNotFound
    };
    template<>
    inline std::string_view Error<IndexErrc>::label(IndexErrc c) {
      switch (c) {
        case IndexErrc::VaultUnspecified:           return "Vault unspecified";
        case IndexErrc::NoPath:                     return "Path unspecified";
        case IndexErrc::SecretExists:               return "Secret already exists";
        case IndexErrc::PathConflict:               return "Path conflict";
        case IndexErrc::SecretNotFound:             return "Secret or path not found";
        case IndexErrc::UndefinedOptional:          return "Undefined optional value";
        case IndexErrc::GpgIdFileNotFound:          return "Gpg ID file not found";
        default:                                    return "Unknown error";
      }
    }
}
