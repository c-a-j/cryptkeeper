#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <memory>
#include <set>
#include <string>
#include <string_view>

#include "lib/crypto/crypto.hpp"
#include "lib/crypto/random.hpp"
#include "lib/crypto/secure_wipe.hpp"
#include "lib/config/types.hpp"
#include "util/error.hpp"

#include "../util/gen_key.hpp"

namespace {
  namespace fs = std::filesystem;

  using ck::crypto::SecureBytes;
  using ck::tests::util::ScopedGnupgHome;
  using ck::tests::util::generate_tmp_key;
  using ck::util::error::CryptoErrc;
  using ck::util::error::Error;
  using enum ck::util::error::CryptoErrc;

  constexpr std::string_view kUppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  constexpr std::string_view kLowercase = "abcdefghijklmnopqrstuvwxyz";
  constexpr std::string_view kNumbers = "0123456789";
  constexpr std::string_view kSymbols = "!@#$%^&*()-_=+[]{};:,.<>?";
  constexpr std::string_view kPasswordChars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789"
    "!@#$%^&*()-_=+[]{};:,.<>?";

  SecureBytes make_bytes(std::string_view value) {
    SecureBytes bytes;
    bytes.assign(value.data(), value.size());
    return bytes;
  }

  std::string_view as_view(const SecureBytes& bytes) {
    if (bytes.size() == 0) {
      return {};
    }
    return {bytes.char_data(), bytes.size()};
  }

  std::string as_string(const SecureBytes& bytes) {
    return std::string(as_view(bytes));
  }

  std::size_t count_chars_in_charset(
    const SecureBytes& bytes,
    std::string_view charset
  ) {
    const std::string_view view = as_view(bytes);
    return static_cast<std::size_t>(std::count_if(
      view.begin(),
      view.end(),
      [&](const char ch) { return charset.find(ch) != std::string_view::npos; }
    ));
  }

  bool contains_only_charset(const SecureBytes& bytes, std::string_view charset) {
    const std::string_view view = as_view(bytes);
    return std::all_of(
      view.begin(),
      view.end(),
      [&](const char ch) { return charset.find(ch) != std::string_view::npos; }
    );
  }

  bool is_lower_hex(const char ch) {
    return ('0' <= ch && ch <= '9') || ('a' <= ch && ch <= 'f');
  }

  template <typename Fn>
  void expect_crypto_error(Fn&& fn, const CryptoErrc expected_code) {
    try {
      std::forward<Fn>(fn)();
      FAIL() << "Expected Error<CryptoErrc>";
    } catch (const Error<CryptoErrc>& err) {
      EXPECT_EQ(err.code, expected_code);
    } catch (const std::exception& err) {
      FAIL() << "Expected Error<CryptoErrc>, got " << err.what();
    } catch (...) {
      FAIL() << "Expected Error<CryptoErrc>";
    }
  }
}

class CryptoGpgTest : public ::testing::Test {
  protected:
    static void SetUpTestSuite() {
      gnupg_home_ = std::make_unique<ScopedGnupgHome>();
      ck::crypto::init_gpgme();
      generated_fpr_ = generate_tmp_key();
    }

    static void TearDownTestSuite() {
      gnupg_home_.reset();
      generated_fpr_.clear();
    }

    static fs::path temp_path(std::string_view filename) {
      return fs::path(gnupg_home_->path()) / std::string(filename);
    }

    static std::unique_ptr<ScopedGnupgHome> gnupg_home_;
    static std::string generated_fpr_;
};

std::unique_ptr<ScopedGnupgHome> CryptoGpgTest::gnupg_home_;
std::string CryptoGpgTest::generated_fpr_;

TEST_F(CryptoGpgTest, InitGpgmeSucceeds) {
  EXPECT_NO_THROW(ck::crypto::init_gpgme());
  EXPECT_NO_THROW(ck::crypto::init_gpgme());
}

TEST_F(CryptoGpgTest, KeyExistsReturnTrue) {
  EXPECT_TRUE(ck::crypto::public_key_exists(generated_fpr_));
  EXPECT_TRUE(ck::crypto::private_key_exists(generated_fpr_));
}

TEST(CryptoStandaloneGpgTest, KeyNotExistsReturnFalse) {
  std::string fpr;
  {
    ScopedGnupgHome gnupg_home;
    ck::crypto::init_gpgme();
    fpr = generate_tmp_key();
  }

  EXPECT_FALSE(ck::crypto::public_key_exists(fpr));
  EXPECT_FALSE(ck::crypto::private_key_exists(fpr));
}

TEST_F(CryptoGpgTest, EncryptBytesRoundTripsSingleRecipient) {
  const std::string plaintext{"s3cr\0t\nvalue", 12};

  const SecureBytes cipher = ck::crypto::encrypt_bytes(
    make_bytes(plaintext),
    generated_fpr_
  );
  const SecureBytes decrypted = ck::crypto::decrypt_bytes(cipher);

  EXPECT_FALSE(cipher.size() == 0);
  EXPECT_NE(as_string(cipher), plaintext);
  EXPECT_EQ(as_string(decrypted), plaintext);
}

TEST_F(CryptoGpgTest, EncryptBytesRoundTripsMultipleRecipients) {
  const std::string plaintext{"multi\0recipient", 15};
  const std::string second_fpr = generate_tmp_key();

  const SecureBytes cipher = ck::crypto::encrypt_bytes(
    make_bytes(plaintext),
    std::vector<std::string>{generated_fpr_, second_fpr}
  );
  const SecureBytes decrypted = ck::crypto::decrypt_bytes(cipher);

  EXPECT_EQ(as_string(decrypted), plaintext);
}

TEST_F(CryptoGpgTest, EncryptBytesThrowsForUnknownRecipient) {
  const std::string plaintext = "secret";
  const std::string missing_fpr = "0123456789abcdef0123456789abcdef01234567";

  expect_crypto_error(
    [&] {
      const auto cipher = ck::crypto::encrypt_bytes(make_bytes(plaintext), missing_fpr);
      (void)cipher;
    },
    GpgmeGetKeyFailed
  );
}

TEST_F(CryptoGpgTest, DecryptBytesThrowsForInvalidCiphertext) {
  expect_crypto_error(
    [&] {
      const auto plain = ck::crypto::decrypt_bytes(make_bytes("not encrypted data"));
      (void)plain;
    },
    GpgmeDataReleaseFailed
  );
}

TEST_F(CryptoGpgTest, ReadFileReturnsExactBytes) {
  const fs::path path = temp_path("raw-bytes.bin");
  const std::array<char, 6> expected{{'\0', 'a', '\n', '\x7f', 'b', '\0'}};
  std::ofstream out(path, std::ios::binary);

  ASSERT_TRUE(out.is_open());
  out.write(expected.data(), static_cast<std::streamsize>(expected.size()));
  ASSERT_TRUE(out.good());
  out.close();

  const SecureBytes actual = ck::crypto::read_file(path);

  EXPECT_EQ(
    as_view(actual),
    std::string_view(expected.data(), expected.size())
  );
}

TEST_F(CryptoGpgTest, ReadFileThrowsForMissingPath) {
  expect_crypto_error(
    [&] {
      const auto bytes = ck::crypto::read_file(temp_path("missing-file.bin"));
      (void)bytes;
    },
    FailedToOpenFile
  );
}

TEST_F(CryptoGpgTest, WriteEncryptsFileThatCanBeDecrypted) {
  const fs::path path = temp_path("written-secret.gpg");
  const std::string plaintext{"path\0based", 10};

  ck::crypto::write(
    make_bytes(plaintext),
    std::vector<std::string>{generated_fpr_},
    path
  );

  ASSERT_TRUE(fs::exists(path));

  const SecureBytes cipher = ck::crypto::read_file(path);
  const SecureBytes decrypted = ck::crypto::decrypt_bytes(cipher);

  EXPECT_NE(as_string(cipher), plaintext);
  EXPECT_EQ(as_string(decrypted), plaintext);
}

TEST_F(CryptoGpgTest, WithDecryptedBytesFromCiphertextReturnsCallbackValue) {
  const std::string plaintext{"callback\0cipher", 15};
  const SecureBytes cipher = ck::crypto::encrypt_bytes(
    make_bytes(plaintext),
    generated_fpr_
  );

  const std::string result = ck::crypto::with_decrypted_bytes(
    cipher,
    [](const SecureBytes& plain) {
      return as_string(plain);
    }
  );

  EXPECT_EQ(result, plaintext);
}

TEST_F(CryptoGpgTest, WithDecryptedBytesFromPathReturnsCallbackValue) {
  const fs::path path = temp_path("callback-secret.gpg");
  const std::string plaintext{"callback\0path", 13};

  ck::crypto::write(
    make_bytes(plaintext),
    std::vector<std::string>{generated_fpr_},
    path
  );

  const std::size_t result = ck::crypto::with_decrypted_bytes(
    path,
    [](const SecureBytes& plain) {
      return plain.size();
    }
  );

  EXPECT_EQ(result, plaintext.size());
}

TEST(CryptoUtilityTest, SecureBytesAssignCopiesInput) {
  const std::string input{"ab\0cd", 5};
  SecureBytes bytes;

  bytes.assign(input.data(), input.size());

  EXPECT_EQ(bytes.size(), input.size());
  EXPECT_EQ(as_view(bytes), std::string_view(input.data(), input.size()));
  ASSERT_EQ(bytes.bytes().size(), input.size());
  EXPECT_EQ(bytes.bytes()[2], std::byte{0});
}

TEST(CryptoUtilityTest, SecureBytesAssignRejectsNullPointerWithLength) {
  SecureBytes bytes;

  expect_crypto_error(
    [&] { bytes.assign(nullptr, 1); },
    InvalidSecureBytesArgs
  );
}

TEST(CryptoUtilityTest, SecureWipeZerosBufferAndAllowsNullOrZeroLength) {
  std::array<unsigned char, 8> buffer{{1, 2, 3, 4, 5, 6, 7, 8}};

  EXPECT_NO_THROW(ck::crypto::secure_wipe(nullptr, 8));
  EXPECT_NO_THROW(ck::crypto::secure_wipe(buffer.data(), 0));

  ck::crypto::secure_wipe(buffer.data(), buffer.size());

  EXPECT_TRUE(std::all_of(
    buffer.begin(),
    buffer.end(),
    [](const unsigned char byte) { return byte == 0; }
  ));
}

TEST(CryptoUtilityTest, UuidV4ProducesCanonicalUniqueValues) {
  std::set<std::string> uuids;

  for (int i = 0; i < 64; ++i) {
    const std::string uuid = ck::crypto::uuid_v4();

    ASSERT_EQ(uuid.size(), 36U);
    EXPECT_EQ(uuid[8], '-');
    EXPECT_EQ(uuid[13], '-');
    EXPECT_EQ(uuid[18], '-');
    EXPECT_EQ(uuid[23], '-');
    EXPECT_EQ(uuid[14], '4');
    EXPECT_TRUE(
      uuid[19] == '8' ||
      uuid[19] == '9' ||
      uuid[19] == 'a' ||
      uuid[19] == 'b'
    );

    for (std::size_t pos = 0; pos < uuid.size(); ++pos) {
      if (pos == 8 || pos == 13 || pos == 18 || pos == 23) {
        continue;
      }
      EXPECT_TRUE(is_lower_hex(uuid[pos])) << uuid;
    }

    uuids.insert(uuid);
  }

  EXPECT_EQ(uuids.size(), 64U);
}

TEST(CryptoUtilityTest, PwgenDefaultsToAllowedCharactersAndLength20) {
  ck::crypto::PwSpec spec = ck::config::Pwgen{}.spec();
  const SecureBytes password = ck::crypto::pwgen(spec);

  EXPECT_EQ(password.size(), 20U);
  EXPECT_TRUE(contains_only_charset(password, kPasswordChars));
}

TEST(CryptoUtilityTest, PwgenHonorsRequestedCharacterCounts) {
  ck::config::Pwgen pwgen {
    .length = 14,
    .symbols = 5,
    .numbers = 4,
    .uppercase = 2,
    .lowercase = 3,
  };
  const SecureBytes password = ck::crypto::pwgen(pwgen.spec());

  EXPECT_EQ(password.size(), 14U);
  EXPECT_EQ(count_chars_in_charset(password, kUppercase), 2U);
  EXPECT_EQ(count_chars_in_charset(password, kLowercase), 3U);
  EXPECT_EQ(count_chars_in_charset(password, kNumbers), 4U);
  EXPECT_EQ(count_chars_in_charset(password, kSymbols), 5U);
}

TEST(CryptoUtilityTest, PwgenRejectsZeroLength) {
  ck::config::Pwgen pwgen {
    .length = 0,
    .symbols = 0,
    .numbers = 0,
    .uppercase = 0,
    .lowercase = 0,
  };
  expect_crypto_error(
    [&] {
      const auto password = ck::crypto::pwgen(pwgen.spec());
      (void)password;
    },
    InvalidPwSpec
  );
}

TEST(CryptoUtilityTest, PwgenRejectsCountsLargerThanLength) {
  ck::config::Pwgen pwgen {
    .length = 4,
    .symbols = 2,
    .numbers = 2,
    .uppercase = 1,
    .lowercase = 0,
  };
  expect_crypto_error(
    [&] {
      const auto password = ck::crypto::pwgen(pwgen.spec());
      (void)password;
    },
    InvalidPwSpec
  );
}
