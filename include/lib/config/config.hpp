#pragma once

#include <expected>
#include <string>
#include <string_view>
#include <vector>

#include "lib/config/error.hpp"
#include "lib/config/pwgen.hpp"

namespace ck::config {
using Err = ck::error::Error<ck::error::ConfigErrc>;

struct Core {
    std::string home;
    bool autopush = true;
    bool autosync = true;
};

struct Ui {
    bool insert_with_editor = false;
};

std::expected<void, Err> validate_pwgen(const PwGen &pwgen);

struct State {
    Core core;
    Ui ui;
    PwGen pwgen;
};

class Config {
  public:
    explicit Config() = default;

    static Config empty();

    const Core &core() const;
    Core &core();

    const Ui &ui() const;
    Ui &ui();

    const PwGen &pwgen() const;
    PwGen &pwgen();

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
