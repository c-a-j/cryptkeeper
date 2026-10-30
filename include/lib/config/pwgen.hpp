#pragma once

namespace ck::config {
struct PwgenCfg {
    int length = 20;
    int symbols = 5;
    int numbers = 5;
    int uppercase = 5;
    int lowercase = 5;
};
} // namespace ck::config
