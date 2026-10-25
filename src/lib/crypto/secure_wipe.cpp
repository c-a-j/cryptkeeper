#include "./lib/crypto/secure_wipe.hpp"

namespace ck::crypto {
void secure_wipe(void *ptr, std::size_t len) {
    if (ptr == nullptr || len == 0) {
        return;
    }

    auto *p = static_cast<volatile unsigned char *>(ptr);
    while (len-- > 0) {
        *p++ = 0;
    }
}
} // namespace ck::crypto
