#pragma once
#include <string_view>

inline constexpr std::string_view APP_NAME = "Crypt Keeper";
inline constexpr std::string_view APP_DIR = "crypt-keeper";
inline constexpr std::string_view CLI_ABBR = "ck";
inline constexpr std::string_view CONFIG_HEADER = "Crypt Keeper Configurations";

inline constexpr std::string_view CONFIG_DIR_ENV_VAR = "CRYPT_KEEPER_CONFIG_DIR";
inline constexpr std::string_view VAULT_DIR_ENV_VAR = "CRYPT_KEEPER_VAULT_DIR";

inline constexpr std::string_view GPG_ID_FILE = ".gpg-id";
inline constexpr std::string_view INDEX_FILE = ".idx";