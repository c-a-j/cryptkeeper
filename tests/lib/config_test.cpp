#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

// include directory includes
#include "lib/config/types.hpp"
#include "util/error.hpp"
#include "util/logger/logger.hpp"

// src directory includes
#include "../util/scoped_config_dir.hpp"
#include "lib/path/existence.hpp"
#include "lib/path/path.hpp"

namespace {
namespace fs = std::filesystem;
using ck::config::Config;

std::string_view bool_str(bool value) {
    return value ? "true" : "false";
}

class StdoutCapture {
  public:
    StdoutCapture() : old_(std::cout.rdbuf(buffer_.rdbuf())) {
    }

    ~StdoutCapture() {
        std::cout.rdbuf(old_);
    }

    std::string str() const {
        return buffer_.str();
    }

  private:
    std::ostringstream buffer_;
    std::streambuf *old_;
};

template <typename Fn>
std::string capture_stdout(Fn &&fn) {
    StdoutCapture capture;
    std::forward<Fn>(fn)();
    return capture.str();
}

void expect_default_values(const Config &cfg) {
    EXPECT_EQ(cfg.core().home, ck::path::crypt_root().string());
    EXPECT_TRUE(cfg.core().autopush);
    EXPECT_TRUE(cfg.core().autosync);
    EXPECT_FALSE(cfg.ui().insert_with_editor);
    EXPECT_EQ(cfg.pwgen().length, 20);
    EXPECT_EQ(cfg.pwgen().symbols, 5);
    EXPECT_EQ(cfg.pwgen().numbers, 5);
    EXPECT_EQ(cfg.pwgen().uppercase, 5);
    EXPECT_EQ(cfg.pwgen().lowercase, 5);
}

std::string format_line(
    std::string_view section,
    std::string_view key,
    std::string_view value
) {
    std::ostringstream out;
    out << "[" << section << "." << key << "] = " << value << "\n";
    return out.str();
}

std::string
format_line(std::string_view section, std::string_view key, const bool value) {
    return format_line(section, key, bool_str(value));
}

template <typename T>
std::string
format_line(std::string_view section, std::string_view key, const T &value) {
    std::ostringstream out;
    out << "[" << section << "." << key << "] = " << value << "\n";
    return out.str();
}
} // namespace

using namespace ck::tests::util;

class ConfigTest : public ::testing::Test {
  protected:
    std::unique_ptr<ScopedConfigDir> config_dir_;

    void SetUp() override {
        config_dir_ = std::make_unique<ScopedConfigDir>();
        ck::util::logger::logger.set_no_color();
        ck::util::logger::logger.set_debug(false);
    }

    void TearDown() override {
        config_dir_.reset();
    }

    fs::path config_file() const {
        return ck::path::config_file();
    }

    void write_config(std::string_view contents) const {
        std::error_code ec;
        fs::create_directories(config_file().parent_path(), ec);
        ASSERT_FALSE(ec);

        std::ofstream out(config_file());
        ASSERT_TRUE(out.is_open());
        out << contents;
        out.close();
        ASSERT_TRUE(out.good());
    }

    void write_config_object(Config &cfg) const {
        capture_stdout([&] { cfg.save(); });
    }
};

TEST_F(ConfigTest, InitializeDefaultConfigNoThrow) {
    Config cfg;
    EXPECT_NO_THROW(cfg.load());
}

TEST_F(ConfigTest, MissingConfigFileUsesDefaults) {
    Config cfg;
    cfg.load();

    expect_default_values(cfg);
    EXPECT_FALSE(ck::path::file_exists(config_file()));
}

TEST_F(ConfigTest, DeserializesValidConfigFile) {
    write_config(R"toml(
[core]
home = "/tmp/test-crypt"
autopush = false
autosync = false

[ui]
insert_with_editor = true

[pwgen]
length = 32
symbols = 7
numbers = 6
uppercase = 8
lowercase = 11
)toml");

    Config cfg;
    cfg.load();

    EXPECT_EQ(cfg.core().home, "/tmp/test-crypt");
    EXPECT_FALSE(cfg.core().autopush);
    EXPECT_FALSE(cfg.core().autosync);
    EXPECT_TRUE(cfg.ui().insert_with_editor);
    EXPECT_EQ(cfg.pwgen().length, 32);
    EXPECT_EQ(cfg.pwgen().symbols, 7);
    EXPECT_EQ(cfg.pwgen().numbers, 6);
    EXPECT_EQ(cfg.pwgen().uppercase, 8);
    EXPECT_EQ(cfg.pwgen().lowercase, 11);
}

TEST_F(ConfigTest, MalformedTomlThrows) {
    write_config(R"toml(
[core
home = "/tmp/test-crypt"
)toml");

    Config cfg;
    EXPECT_THROW(cfg.load(), ck::error::AppError);
}

TEST_F(ConfigTest, MissingSectionUsesDefaults) {
    write_config(R"toml(
[core]
home = "/tmp/test-crypt"
autopush = false
autosync = false

[pwgen]
length = 30
symbols = 4
numbers = 4
uppercase = 4
lowercase = 18
)toml");

    Config cfg;
    cfg.load();

    EXPECT_EQ(cfg.core().home, "/tmp/test-crypt");
    EXPECT_FALSE(cfg.core().autopush);
    EXPECT_FALSE(cfg.core().autosync);
    EXPECT_FALSE(cfg.ui().insert_with_editor);
    EXPECT_EQ(cfg.pwgen().length, 30);
    EXPECT_EQ(cfg.pwgen().symbols, 4);
    EXPECT_EQ(cfg.pwgen().numbers, 4);
    EXPECT_EQ(cfg.pwgen().uppercase, 4);
    EXPECT_EQ(cfg.pwgen().lowercase, 18);
}

TEST_F(ConfigTest, NonTableSectionUsesDefaults) {
    write_config(R"toml(
[core]
home = "/tmp/test-crypt"
autopush = false
autosync = false

ui = true

[pwgen]
length = 30
symbols = 4
numbers = 4
uppercase = 4
lowercase = 18
)toml");

    Config cfg;
    cfg.load();

    EXPECT_FALSE(cfg.core().autopush);
    EXPECT_FALSE(cfg.core().autosync);
    EXPECT_FALSE(cfg.ui().insert_with_editor);
    EXPECT_EQ(cfg.pwgen().length, 30);
}

TEST_F(ConfigTest, MissingFieldUsesDefault) {
    write_config(R"toml(
[core]
home = "/tmp/test-crypt"
autopush = false
autosync = false

[ui]
insert_with_editor = true

[pwgen]
length = 31
symbols = 6
uppercase = 9
lowercase = 10
)toml");

    Config cfg;
    cfg.load();

    EXPECT_EQ(cfg.pwgen().length, 31);
    EXPECT_EQ(cfg.pwgen().symbols, 6);
    EXPECT_EQ(cfg.pwgen().numbers, 5);
    EXPECT_EQ(cfg.pwgen().uppercase, 9);
    EXPECT_EQ(cfg.pwgen().lowercase, 10);
}

TEST_F(ConfigTest, WrongTypedFieldUsesDefault) {
    write_config(R"toml(
[core]
home = "/tmp/test-crypt"
autopush = false
autosync = "false"

[ui]
insert_with_editor = "true"

[pwgen]
length = "abc"
symbols = 6
numbers = 7
uppercase = 8
lowercase = 9
)toml");

    Config cfg;
    cfg.load();

    EXPECT_EQ(cfg.core().home, "/tmp/test-crypt");
    EXPECT_FALSE(cfg.core().autopush);
    EXPECT_TRUE(cfg.core().autosync);
    EXPECT_FALSE(cfg.ui().insert_with_editor);
    EXPECT_EQ(cfg.pwgen().length, 20);
    EXPECT_EQ(cfg.pwgen().symbols, 5);
    EXPECT_EQ(cfg.pwgen().numbers, 5);
    EXPECT_EQ(cfg.pwgen().uppercase, 5);
    EXPECT_EQ(cfg.pwgen().lowercase, 5);
}

TEST_F(ConfigTest, MissingCoreHomeMakesConstHomeThrow) {
    write_config(R"toml(
[core]
autopush = false
autosync = false

[ui]
insert_with_editor = true

[pwgen]
length = 25
symbols = 5
numbers = 5
uppercase = 5
lowercase = 10
)toml");

    Config cfg;
    cfg.load();
    const Config &const_cfg = cfg;

    EXPECT_TRUE(cfg.core().home.empty());
    EXPECT_THROW(const_cfg.home(), ck::error::AppError);
}

TEST_F(ConfigTest, MutableHomeBackfillsCryptRootWhenMissing) {
    write_config(R"toml(
[core]
autopush = false
autosync = false

[ui]
insert_with_editor = true

[pwgen]
length = 25
symbols = 5
numbers = 5
uppercase = 5
lowercase = 10
)toml");

    Config cfg;
    cfg.load();

    EXPECT_TRUE(cfg.core().home.empty());
    EXPECT_EQ(cfg.home(), ck::path::crypt_root().string());
    EXPECT_EQ(cfg.core().home, ck::path::crypt_root().string());
}

TEST_F(ConfigTest, SetStringFieldUpdatesValue) {
    Config cfg;
    cfg.load();

    cfg.set({"core.home", "/tmp/override-crypt"});

    EXPECT_EQ(cfg.core().home, "/tmp/override-crypt");
}

TEST_F(ConfigTest, SetBoolFieldUpdatesTrueAndFalse) {
    Config cfg;
    cfg.load();

    cfg.set({"core.autopush", "false"});
    cfg.set({"ui.insert_with_editor", "true"});

    EXPECT_FALSE(cfg.core().autopush);
    EXPECT_TRUE(cfg.ui().insert_with_editor);
}

TEST_F(ConfigTest, SetIntFieldUpdatesValue) {
    Config cfg;
    cfg.load();

    cfg.set({"pwgen.length", "48"});

    EXPECT_EQ(cfg.pwgen().length, 48);
}

TEST_F(ConfigTest, SetRejectsUnknownSection) {
    Config cfg;
    cfg.load();

    EXPECT_THROW(cfg.set({"foobar.length", "48"}), ck::error::AppError);
}

TEST_F(ConfigTest, SetRejectsUnknownMember) {
    Config cfg;
    cfg.load();

    EXPECT_THROW(cfg.set({"core.foobar", "true"}), ck::error::AppError);
}

TEST_F(ConfigTest, SetRejectsMalformedKeys) {
    Config cfg;
    cfg.load();
    const std::vector<std::string> invalid_keys{
        "core",
        "core.home.extra",
        ".home",
        "core.",
    };

    for (const auto &key : invalid_keys) {
        SCOPED_TRACE(key);
        EXPECT_THROW(cfg.set({key, "true"}), ck::error::AppError);
    }
}

TEST_F(ConfigTest, SetRejectsInvalidInteger) {
    Config cfg;
    cfg.load();
    const std::vector<std::string> invalid_values{
        "foobar",
        "12abc",
        "999999999999999999999999999999",
    };

    for (const auto &value : invalid_values) {
        SCOPED_TRACE(value);
        EXPECT_THROW(cfg.set({"pwgen.length", value}), ck::error::AppError);
    }
}

TEST_F(ConfigTest, WriteCreatesConfigDirAndFile) {
    Config cfg;
    cfg.load();

    write_config_object(cfg);

    EXPECT_TRUE(ck::path::directory_exists(ck::path::config_dir()));
    EXPECT_TRUE(ck::path::file_exists(config_file()));
}

TEST_F(ConfigTest, WritePersistsValuesAcrossReload) {
    Config cfg;
    cfg.load();
    cfg.set({"core.home", "/tmp/round-trip-crypt"});
    cfg.set({"core.autopush", "false"});
    cfg.set({"ui.insert_with_editor", "true"});
    cfg.set({"pwgen.length", "28"});
    cfg.set({"pwgen.symbols", "3"});

    write_config_object(cfg);

    Config reloaded;
    reloaded.load();

    EXPECT_EQ(reloaded.core().home, "/tmp/round-trip-crypt");
    EXPECT_FALSE(reloaded.core().autopush);
    EXPECT_TRUE(reloaded.ui().insert_with_editor);
    EXPECT_EQ(reloaded.pwgen().length, 28);
    EXPECT_EQ(reloaded.pwgen().symbols, 3);
}

TEST_F(ConfigTest, WriteOverwritesExistingConfig) {
    Config cfg;
    cfg.load();
    cfg.set({"core.home", "/tmp/first-crypt"});
    cfg.set({"pwgen.length", "22"});
    write_config_object(cfg);

    cfg.set({"core.home", "/tmp/second-crypt"});
    cfg.set({"core.autosync", "false"});
    cfg.set({"pwgen.length", "42"});
    write_config_object(cfg);

    Config reloaded;
    reloaded.load();

    EXPECT_EQ(reloaded.core().home, "/tmp/second-crypt");
    EXPECT_FALSE(reloaded.core().autosync);
    EXPECT_EQ(reloaded.pwgen().length, 42);
}

TEST_F(ConfigTest, WriteDefaultConfigIsParseable) {
    Config cfg;
    cfg.load();

    write_config_object(cfg);

    EXPECT_NO_THROW({
        Config reloaded;
        reloaded.load();
        expect_default_values(reloaded);
    });
}

TEST_F(ConfigTest, PrintAllOutputsEveryField) {
    Config cfg;
    cfg.load();
    cfg.set({"core.home", "/tmp/printed-crypt"});
    cfg.set({"core.autopush", "false"});
    cfg.set({"core.autosync", "false"});
    cfg.set({"ui.insert_with_editor", "true"});
    cfg.set({"pwgen.length", "24"});
    cfg.set({"pwgen.symbols", "4"});
    cfg.set({"pwgen.numbers", "5"});
    cfg.set({"pwgen.uppercase", "6"});
    cfg.set({"pwgen.lowercase", "9"});

    const std::string output = capture_stdout([&] { cfg.print(); });

    const std::string expected =
        format_line("core", "home", "/tmp/printed-crypt") +
        format_line("core", "autopush", false) +
        format_line("core", "autosync", false) +
        format_line("ui", "insert_with_editor", true) +
        format_line("pwgen", "length", 24) +
        format_line("pwgen", "symbols", 4) +
        format_line("pwgen", "numbers", 5) +
        format_line("pwgen", "uppercase", 6) +
        format_line("pwgen", "lowercase", 9);

    EXPECT_EQ(output, expected);
}

TEST_F(ConfigTest, PrintSectionOutputsOnlySection) {
    Config cfg;
    cfg.load();
    cfg.set({"pwgen.length", "24"});
    cfg.set({"pwgen.symbols", "4"});
    cfg.set({"pwgen.numbers", "5"});
    cfg.set({"pwgen.uppercase", "6"});
    cfg.set({"pwgen.lowercase", "9"});

    const std::string output = capture_stdout([&] { cfg.print("pwgen"); });

    const std::string expected = format_line("pwgen", "length", 24) +
        format_line("pwgen", "symbols", 4) +
        format_line("pwgen", "numbers", 5) +
        format_line("pwgen", "uppercase", 6) +
        format_line("pwgen", "lowercase", 9);

    EXPECT_EQ(output, expected);
}

TEST_F(ConfigTest, PrintKeyOutputsSingleLine) {
    Config cfg;
    cfg.load();
    cfg.set({"core.home", "/tmp/printed-crypt"});

    const std::string output = capture_stdout([&] { cfg.print("core.home"); });

    EXPECT_EQ(output, format_line("core", "home", "/tmp/printed-crypt"));
}

TEST_F(ConfigTest, PrintRejectsUnknownKeys) {
    Config cfg;
    cfg.load();
    const std::vector<std::string> invalid_keys{
        "nope",
        "core.nope",
        "core.home.extra",
    };

    for (const auto &key : invalid_keys) {
        SCOPED_TRACE(key);
        EXPECT_THROW(cfg.print(key), ck::error::AppError);
    }
}
