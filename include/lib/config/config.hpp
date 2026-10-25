#pragma once

#include <expected>
#include <string>
#include <string_view>
#include <vector>

#include "lib/config/error.hpp"
#include "lib/crypto/types.hpp"

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

struct Pwgen {
    int length = 20;
    int symbols = 5;
    int numbers = 5;
    int uppercase = 5;
    int lowercase = 5;

    std::expected<void, Err> valid() const;
    ck::crypto::PwSpec spec() const;
};

struct State {
    Core core;
    Ui ui;
    Pwgen pwgen;

    std::expected<void, Err> valid_pwgen() const;
};

class Config {
  public:
    explicit Config() = default;

    static Config empty();

    const Core &core() const;
    Core &core();

    const Ui &ui() const;
    Ui &ui();

    const Pwgen &pwgen() const;
    Pwgen &pwgen();

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
