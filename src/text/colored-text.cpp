#include "paracl/text/colored-text.h"

#include <cassert>
#include <print>


namespace paracl {



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
