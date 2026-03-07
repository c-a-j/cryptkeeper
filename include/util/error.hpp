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
      case InitErrc::KeyNotFound:           return "Public key not found: ";
      case InitErrc::CreateDirectoryFailed: return "Failed to create vault: ";
      case InitErrc::OpenGpgIdFailed:       return "Failed to open .gpg-id: ";
      case InitErrc::WriteGpgIdFailed:      return "Failed to write .gpg-id: ";
      case InitErrc::AlreadyExists:         return "Vault already exists: ";
      case InitErrc::IoError:               return "I/O error: ";
      default:                              return "Unknown error: ";
    }
  }
  
  enum class ConfigErrc {
    CreateDirectoryFailed,
    AlreadyExists,
    DoesNotExist,
    InvalidScope,
    IoError,
  };
  template<>
  inline std::string_view Error<ConfigErrc>::label(ConfigErrc c) {
    switch (c) {
      case ConfigErrc::CreateDirectoryFailed: return "Failed to create vault: ";
      case ConfigErrc::AlreadyExists:         return "Configuration file already exists: ";
      case ConfigErrc::DoesNotExist:          return "Configuration file not found: ";
      case ConfigErrc::InvalidScope:          return "Invalid configuration spec: ";
      case ConfigErrc::IoError:               return "I/O error: ";
      default:                                return "Unknown error: ";
    }
  }
  //   struct ConfigError : Error { using Error::Error; };
  //   struct VaultError  : Error { using Error::Error; };
  //   struct CryptoError : Error { using Error::Error; };
}