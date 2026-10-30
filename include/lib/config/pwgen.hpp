#pragma once

namespace ck::config {
struct PwGen {
    int length = 20;
    int symbols = 5;
    int numbers = 5;
    int uppercase = 5;
    int lowercase = 5;
};
}
