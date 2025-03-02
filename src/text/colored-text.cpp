#include "paracl/text/colored-text.h"

#include <cassert>
#include <print>

#ifdef _WIN32
#include <io.h>
#define isatty _isatty
#define fileno _fileno
#else
#include <unistd.h>
#endif


namespace paracl {

namespace {

constexpr std::string RESET_SEQUENCE = "\033[0m";

} // end anonymous namespace

void colored_text::clear_formatting() {
    if (current_overlay_) {
        if (current_overlay_->begin != text_.size()) {
            current_overlay_->end = text_.size();
            overlays_.push_back(*current_overlay_);
        }

        current_overlay_ = std::nullopt;
    }
}

void colored_text::set_formatting(colored_text::formatting formatting) {
    clear_formatting();
    current_overlay_ = {formatting, text_.size(), text_.size()};
}

void colored_text::set_foreground(colored_text::color foreground) {
    set_formatting({ .foreground_color = foreground });
}

void colored_text::set_background(colored_text::color background) {
    set_formatting({ .background_color = background });
}

void colored_text::set_attribute(colored_text::attribute attribute) {
    set_formatting({ .attribute = attribute });
}

void colored_text::print() {
    bool should_colorize = isatty(fileno(stdout));
    bool should_reset = false;

    size_t overlay_index = 0;
    for (size_t i = 0; i < text_.size(); ++ i) {
        if (should_colorize && overlay_index < overlays_.size()) {
            if (overlays_[overlay_index].begin == i) {
                std::print("{}",
                    overlays_[overlay_index].formatting.get_ansi_code()
                );

                should_reset = true;
            }
        }

        std::print("{}", text_[i]);

        if (should_colorize && overlay_index < overlays_.size()) {
            if (overlays_[overlay_index].end == i + 1) {
                assert(should_reset && "overlay ended but haven't begun");
                should_reset = false;

                std::print("{}", RESET_SEQUENCE);
                overlay_index ++;
            }
        }
    }
}


std::string colored_text::color::get_foreground_ansi_code() const {
    return std::visit([](auto&& it) {
        return it.get_foreground_ansi_code();
    }, color_);
}

std::string colored_text::color::get_background_ansi_code() const {
    return std::visit([](auto&& it) {
        return it.get_background_ansi_code();
    }, color_);
}

bool colored_text::color::operator==(color other) const {
    return color_ == other.color_;
}


std::string colored_text::color::preset::get_foreground_ansi_code() const {
    switch (kind_) {
    case BLACK:          return "\033[30m";
    case RED:            return "\033[31m";
    case GREEN:          return "\033[32m";
    case YELLOW:         return "\033[33m";
    case BLUE:           return "\033[34m";
    case MAGENTA:        return "\033[35m";
    case CYAN:           return "\033[36m";
    case WHITE:          return "\033[37m";
    case BRIGHT_BLACK:   return "\033[90m";
    case BRIGHT_RED:     return "\033[91m";
    case BRIGHT_GREEN:   return "\033[92m";
    case BRIGHT_YELLOW:  return "\033[93m";
    case BRIGHT_BLUE:    return "\033[94m";
    case BRIGHT_MAGENTA: return "\033[95m";
    case BRIGHT_CYAN:    return "\033[96m";
    case BRIGHT_WHITE:   return "\033[97m";
    }
}

std::string colored_text::color::preset::get_background_ansi_code() const {
    switch (kind_) {
    case BLACK:          return "\033[40m";
    case RED:            return "\033[41m";
    case GREEN:          return "\033[42m";
    case YELLOW:         return "\033[43m";
    case BLUE:           return "\033[44m";
    case MAGENTA:        return "\033[45m";
    case CYAN:           return "\033[46m";
    case WHITE:          return "\033[47m";
    case BRIGHT_BLACK:   return "\033[100m";
    case BRIGHT_RED:     return "\033[101m";
    case BRIGHT_GREEN:   return "\033[102m";
    case BRIGHT_YELLOW:  return "\033[103m";
    case BRIGHT_BLUE:    return "\033[104m";
    case BRIGHT_MAGENTA: return "\033[105m";
    case BRIGHT_CYAN:    return "\033[106m";
    case BRIGHT_WHITE:   return "\033[107m";
    }
}

bool colored_text::color::preset::operator==(color::preset other) const {
    return kind_ == other.kind_;
}


std::string colored_text::color::rgb::get_foreground_ansi_code() const {
    return std::format("\033[38;2;{};{};{}m", r_, g_, b_);
}

std::string colored_text::color::rgb::get_background_ansi_code() const {
    return std::format("\033[48;2;{};{};{}m", r_, g_, b_);
}

bool colored_text::color::rgb::operator==(color::rgb other) const {
    return r_ == other.r_ && g_ == other.g_ && b_ == other.b_;
}


std::string colored_text::attribute::get_ansi_code() const {
    switch (kind_) {
        case BOLD:           return "\033[1m";
        case DIM:            return "\033[2m";
        case ITALIC:         return "\033[3m";
        case UNDERLINE:      return "\033[4m";
        case BLINK:          return "\033[5m";
        case INVERSE:        return "\033[7m";
        case STRIKETHROUGH:  return "\033[9m";
    }
}


std::string colored_text::formatting::get_ansi_code() const {
    std::string ansi_code;

    if (foreground_color)
        ansi_code += foreground_color->get_foreground_ansi_code();

    if (background_color)
        ansi_code += background_color->get_background_ansi_code();

    if (attribute)
        ansi_code += attribute->get_ansi_code();

    return ansi_code;
}

} // end namespace paracl
