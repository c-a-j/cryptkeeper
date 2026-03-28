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
    OpenIndexFailed,
    AlreadyExists,
    IoError,
    NoPathToIndex,
    WriteIndexFailed
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
      case InitErrc::OpenIndexFailed:       return "Failed to open index file";
      case InitErrc::WriteIndexFailed:      return "Failed to create a new index file";
      default:                              return "Unknown error";
    }
  }
  
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
  template<>
  inline std::string_view Error<ConfigErrc>::label(ConfigErrc c) {
    switch (c) {
      case ConfigErrc::CreateDirectoryFailed: return "Failed to create vault";
      case ConfigErrc::CreateConfigFailed:    return "Failed to create conifig file";
      case ConfigErrc::WriteConfigFailed:     return "Failed to write conifig file";
      case ConfigErrc::AlreadyExists:         return "Configuration file already exists";
      case ConfigErrc::DoesNotExist:          return "Configuration file not found";
      case ConfigErrc::InvalidSetParameter:   return "Invalid configuration scope";
      case ConfigErrc::InvalidConfigFile:     return "Invalid configuration file";
      case ConfigErrc::InvalidConfigKey:      return "Invalid configuration key";
      case ConfigErrc::InvalidConfigValue:    return "Invalid configuration value";
      case ConfigErrc::IoError:               return "I/O error";
      default:                                return "Unknown error";
    }
  }
  enum class MountErrc {
    MountFileNotFound,
    WriteMountFailed,
    VaultNotInitialized,
    NoAlias,
    AliasExists,
    AliasDoesNotExist,
    NoRoot,
    InvalidArguments,
    InvalidMountFile,
    SecretAlongAlias,
  };
  template<>
  inline std::string_view Error<MountErrc>::label(MountErrc c) {
    switch (c) {
      case MountErrc::MountFileNotFound:      return "Mount file not found";
      case MountErrc::WriteMountFailed:       return "Failed to write mount file";
      case MountErrc::VaultNotInitialized:    return "Vault is not initialized";
      case MountErrc::NoAlias:                return "Invalid mount file, every mount needs an alias";
      case MountErrc::AliasExists:            return "Alias already exists";
      case MountErrc::AliasDoesNotExist:      return "Alias does not exist";
      case MountErrc::NoRoot:                 return "There is no root vault";
      case MountErrc::InvalidArguments:       return "Invalid arguments";
      case MountErrc::InvalidMountFile:       return "Invalid mount file";
      case MountErrc::SecretAlongAlias:       return "There is a secret along this alias path";
      default:                                return "Unknown error";
    }
  }
    
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
  template<>
  inline std::string_view Error<CryptoErrc>::label(CryptoErrc c) {
    switch (c) {
      case CryptoErrc::FillRandomBytesFailed:     return "Fill random bytes failure";
      case CryptoErrc::GpgmeFailed:               return "gpgme failure";
      case CryptoErrc::GpgmeDataNewFailed:        return "gpgme_data_new failure";
      case CryptoErrc::GpgmeDataNewFromMemFailed: return "gpgme_data_new_from_mem failure";
      case CryptoErrc::GpgmeDataReleaseFailed:    return "gpgme_data_release failure";
      case CryptoErrc::GpgmeDataReleaseAndGetMemFailed: return "gpgme_data_release_and_get_mem failure";
      case CryptoErrc::GpgmeNewFailed:            return "gpgme_new failure";
      case CryptoErrc::GpgmeNewFromMemFailed:     return "gpgme_new_from_mem failure";
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
    GpgIdFileNotFound,
    OpenIndexFailed,
    IndexFileNotFound,
    NoPathToIndex,
    InvalidIndexFile,
    InvalidOrEmptyIndexFile,
    CorruptedIndex
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
      case IndexErrc::OpenIndexFailed:            return "Failed to open index file";
      case IndexErrc::IndexFileNotFound:          return "Index file not found";
      case IndexErrc::NoPathToIndex:              return "No path to index file";
      case IndexErrc::CorruptedIndex:             return "Corrupted index";
      case IndexErrc::InvalidIndexFile:           return "Corrupted index";
      case IndexErrc::InvalidOrEmptyIndexFile:    return "Invalid or empty index file";
      default:                                    return "Unknown error";
    }
  }

  enum class PathErrc {
    CreateDirectoryFailed,
    DataHomeNotSet,
    DirectoryDoesNotExist,
    DoesNotExist,
    FileDoesNotExist,
    FileSystemError,
    HomeNotSet,
    LocalAppDataNotSet,
    VaultUnspecified,
  };
  template<>
  inline std::string_view Error<PathErrc>::label(PathErrc c) {
    switch (c) {
      case PathErrc::CreateDirectoryFailed:   return "Failed to create config directory";
      case PathErrc::DataHomeNotSet:          return "XDG_DATA_HOME not set";
      case PathErrc::DirectoryDoesNotExist:   return "Directory does not exist";
      case PathErrc::DoesNotExist:            return "Directory or file does not exist";
      case PathErrc::FileDoesNotExist:        return "File does not exist";
      case PathErrc::FileSystemError:         return "Filesystem error";
      case PathErrc::HomeNotSet:              return "HOME not set";
      case PathErrc::LocalAppDataNotSet:      return "LOCALAPPDATA not set";
      case PathErrc::VaultUnspecified:        return "Vault unspecified";
      default:                                return "Unknown error";
    }
  }

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
  template<>
  inline std::string_view Error<FsErrc>::label(FsErrc c) {
    switch (c) {
      case FsErrc::CloseFailed:             return "Failed to close file";
      case FsErrc::CreateDirectoryFailed:   return "Failed to create config directory";
      case FsErrc::FileSystemError:         return "Filesystem error";
      case FsErrc::FsyncFailed:             return "Fsync failed";
      case FsErrc::MkstempFailed:           return "mkstemp failed";
      case FsErrc::OpenDirectoryFailed:     return "Failed to open directory";
      case FsErrc::OpenFailed:              return "Failed to open file";
      case FsErrc::RenameFailed:            return "Failed to rename file";
      case FsErrc::SyncDirectoryFailed:     return "Failed to sync directory";
      case FsErrc::WriteFailed:             return "Failed to write to file";
      default:                              return "Unknown error";
    }
  }
}
