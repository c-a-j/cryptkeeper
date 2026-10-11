#pragma once
#include <string_view>

// this is the name of entries in the toml index
// appear as [[OBJ_ARR_NAME]], ex [[entry]], or [[secret]] in the index file
// defined here for consistency because it is used in write and deserialize
inline constexpr std::string_view IDX_ARR_NAME = "entry";