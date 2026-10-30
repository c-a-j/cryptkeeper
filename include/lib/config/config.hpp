#pragma once

#include <expected>
#include <string>
#include <string_view>
#include <vector>

#include "lib/config/error.hpp"
#include "lib/config/pwgen.hpp"

namespace ck::config {
using Err = ck::error::Error<ck::error::ConfigErrc>;

struct CoreCfg {
    std::string home;
    bool autopush = true;
    bool autosync = true;
};

struct UiCfg {
    bool insert_with_editor = false;
};

std::expected<void, Err> validate_pwgen(const PwgenCfg &pwgen);

struct State {
    CoreCfg core;
    UiCfg ui;
    PwgenCfg pwgen;
};

class Config {
  public:
    explicit Config() = default;

    static Config empty();

    const CoreCfg &core() const;
    CoreCfg &core();

    const UiCfg &ui() const;
    UiCfg &ui();

    const PwgenCfg &pwgen() const;
    PwgenCfg &pwgen();

    const std::string &home() const;
    std::string &home();

    void print() const;
    void print(std::string_view key) const;
    void set(std::vector<std::string> args);

    void load();
    void save();

  private:
    State state_;
};

extern Config cfg;
} // namespace ck::config
