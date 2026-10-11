#pragma once

#include <error.hpp>

namespace ck::error {
  enum class IndexErrc {
    CryptUnspecified,
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
      case IndexErrc::CryptUnspecified:           return "Crypt unspecified";
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
      case IndexErrc::InvalidIndexFile:           return "Invalid index file";
      case IndexErrc::InvalidOrEmptyIndexFile:    return "Invalid or empty index file";
      default:                                    return "Unknown error";
    }
  }
}
