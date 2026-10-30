#include <filesystem>
#include <sstream>

#include "toml++/toml.hpp"

#include "../path/internal/path.hpp"
#include "./internal/codec.hpp"
#include "error.hpp"
#include "lib/config/config.hpp"
#include "lib/fio/atomic_write.hpp"
#include "lib/path/existence.hpp"
#include "ui/console/console.hpp"

namespace ck::config {
using ck::error::ConfigErrc;
using ck::error::Error;
using enum ck::error::ConfigErrc;
using ck::ui::console::console;

void Config::load() {
    std::filesystem::path cfg_file = ck::path::config_file();
    if (!ck::path::file_exists(cfg_file)) {
        this->state_.core.home = ck::path::crypt_root();
        return;
    }

    std::ifstream in(cfg_file, std::ios::binary);
    std::string text(
        (std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>()
    );

    State state = codec::deserialize(text);
    this->state_ = state;
}

void Config::save() {
    ck::path::create_config_dir();

    State state = this->state_;

    toml::table tbl = codec::serialize(state);
    std::filesystem::path cfg_file = ck::path::config_file();

    std::error_code ec;
    bool existed = std::filesystem::exists(cfg_file, ec);
    if (ec) {
        existed = false;
    }

    std::ostringstream contents;
    contents << tbl << "\n";

    console.debug("Writing config file");
    ck::fio::atomic_write(cfg_file, contents.str());

    if (!existed) {
        console.info("Created new config file", cfg_file.string());
    } else {
        console.info("Updated config file", cfg_file.string());
    }
    this->print();
}
} // namespace ck::config
