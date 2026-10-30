#include "ui/console/console.hpp"

#include <iostream>

#include "global.hpp"
#include "ui/console/theme.hpp"
#include "ui/term.hpp"

namespace ck::ui::console {
using namespace ck::ui::term;
void Console::emit(Level level, std::string_view msg, Overrides o) {
    if (!theme_) {
        const Stream stream = o.stream.value_or(get_level_stream(level));
        std::ostream &out = (stream == Stream::Out) ? std::cout : std::cerr;

        if (o.prefix) {
            out << CLI_ABBR << "  " << get_level_tag(level) << "  ";
        }

        out << msg;
        if (o.newline) {
            out << "\n";
        }
        return;
    }

    const LevelStyle &s = style_for(*theme_, level);
    std::string color_code =
        (!o.style) ? ansi(s.colors.primary) : ansi(*o.style);

    std::string prefix_color_code = ansi(s.colors.prefix);

    Stream stream = o.stream.value_or(s.stream);

    std::ostream &out = (stream == Stream::Out) ? std::cout : std::cerr;

    if (cfg_.no_color) {
        color_code = "";
        prefix_color_code = "";
    }

    if (o.prefix) {
        out << prefix_color_code << CLI_ABBR << "  " << reset() << color_code
            << s.tag << reset() << "  ";
    }

    out << color_code << msg << reset();
    if (o.newline) {
        out << "\n";
    }
}

// This console.function is for printing messages with two colors so only the
// most important part (the first part) is colorful.
void Console::emit(Level level, std::string_view msg1, std::string_view msg2) {
    Overrides secondary;
    secondary.prefix = false;

    if (theme_) {
        secondary.style = style_for(*theme_, level).colors.secondary;
    }

    emit(level, std::string(msg1) + ": ", Overrides{}.set_newline(false));
    emit(level, msg2, std::move(secondary));
}

void Console::info(std::string_view msg, Overrides o) {
    emit(Level::Info, msg, std::move(o));
}
void Console::info(std::string_view msg1, std::string_view msg2) {
    emit(Level::Info, msg1, msg2);
}

void Console::success(std::string_view msg, Overrides o) {
    emit(Level::Success, msg, std::move(o));
}
void Console::success(std::string_view msg1, std::string_view msg2) {
    emit(Level::Success, msg1, msg2);
}

void Console::warning(std::string_view msg, Overrides o) {
    emit(Level::Warning, msg, std::move(o));
}
void Console::warning(std::string_view msg1, std::string_view msg2) {
    emit(Level::Warning, msg1, msg2);
}

void Console::error(std::string_view msg, Overrides o) {
    emit(Level::Error, msg, std::move(o));
}
void Console::error(std::string_view msg1, std::string_view msg2) {
    emit(Level::Error, msg1, msg2);
}

void Console::debug(std::string_view msg, Overrides o) {
    if (!cfg_.debug) {
        return;
    }
    emit(Level::Debug, msg, std::move(o));
}
void Console::debug(std::string_view msg1, std::string_view msg2) {
    if (!cfg_.debug) {
        return;
    }
    emit(Level::Debug, msg1, msg2);
}

void Console::configure(const ConsoleConfig &cfg) {
    cfg_ = cfg;
}

void Console::set_no_color() {
    cfg_.no_color = true;
}

void Console::set_debug(const bool debug) {
    cfg_.debug = debug;
}

void Console::initialize(Theme theme) {
    theme_.emplace(std::move(theme));
}

void Console::initialize() {
    theme_.emplace(default_theme());
}

constinit Console console;
} // namespace ck::ui::console
