#pragma once
#include <string_view>

inline constexpr std::string_view APP_NAME = "Crypt Keeper";
inline constexpr std::string_view APP_DIR = "crypt-keeper";
inline constexpr std::string_view CLI_ABBR = "ck";

inline constexpr const char *CONFIG_DIR_ENV_VAR = "CRYPTKEEPER_CONFIG_DIR";
inline constexpr const char *CRYPT_DIR_ENV_VAR = "CRYPTKEEPER_CRYPT_DIR";

// for windows
// config dir = LOCALAPPDATA/APP_DIR/config
// crypt home = LOCALAPPDATA/APP_DIR/COLLECTION_HOME_DIR
inline constexpr std::string_view COLLECTION_HOME_DIR = "crypts";
inline constexpr std::string_view COLLECTION_NAME = "crypt";

inline constexpr std::string_view CONFIG_FILE = "config.toml";
inline constexpr std::string_view MOUNT_FILE = "mount.toml";
inline constexpr std::string_view GPG_ID_FILE = ".gpg-id";
inline constexpr std::string_view INDEX_FILE = ".idx.gpg";
