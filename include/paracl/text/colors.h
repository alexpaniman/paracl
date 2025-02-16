#pragma once

#include <cstdint>
#include <variant>
#include <string>
#include <format>
#include <vector>
#include <iostream>

// TODO: should support windows?
#include <unistd.h>


namespace paracl {

class rgb_ansi_color {
public:
    rgb_ansi_color(uint8_t r, uint8_t g, uint8_t b):
        r_(r),
        g_(g),
        b_(b) {
    }

    std::string get_foreground_ansi_code() const {
        return std::format("\033[38;2;{};{};{}m", r_, g_, b_);

    }

    std::string get_background_ansi_code() const {
        return std::format("\033[48;2;{};{};{}m", r_, g_, b_);
    }

    bool operator==(rgb_ansi_color other) const {
        return r_ == other.r_ && g_ == other.g_ && b_ == other.b_;
    }

private:
    uint8_t r_, g_, b_;
};

class ansi_preset_color {
public:
    enum its_kind {
        BLACK,
        RED,
        GREEN,
        YELLOW,
        BLUE,
        MAGENTA,
        CYAN,
        WHITE,
        BRIGHT_BLACK,
        BRIGHT_RED,
        BRIGHT_GREEN,
        BRIGHT_YELLOW,
        BRIGHT_BLUE,
        BRIGHT_MAGENTA,
        BRIGHT_CYAN,
        BRIGHT_WHITE
    };

    ansi_preset_color(its_kind kind):
        kind_(kind) {
    }

    std::string get_foreground_ansi_code() const {
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

    std::string get_background_ansi_code() const {
        switch (kind_) {
        case BLACK:         return "\033[40m";
        case RED:           return "\033[41m";
        case GREEN:         return "\033[42m";
        case YELLOW:        return "\033[43m";
        case BLUE:          return "\033[44m";
        case MAGENTA:       return "\033[45m";
        case CYAN:          return "\033[46m";
        case WHITE:         return "\033[47m";
        case BRIGHT_BLACK:  return "\033[100m";
        case BRIGHT_RED:    return "\033[101m";
        case BRIGHT_GREEN:  return "\033[102m";
        case BRIGHT_YELLOW: return "\033[103m";
        case BRIGHT_BLUE:   return "\033[104m";
        case BRIGHT_MAGENTA:return "\033[105m";
        case BRIGHT_CYAN:   return "\033[106m";
        case BRIGHT_WHITE:  return "\033[107m";
        }
    }

    bool operator==(ansi_preset_color other) const {
        return kind_ == other.kind_;
    }

private:
    its_kind kind_;
};

class ansi_color {
public:
    ansi_color(ansi_preset_color::its_kind kind):
        color_(ansi_preset_color(kind)) {
    }

    ansi_color(uint8_t r, uint8_t g, uint8_t b):
        color_(rgb_ansi_color(r, g, b)) {
    }

    std::string get_foreground_ansi_code() const {
        return std::visit([](auto&& it){
            return it.get_foreground_ansi_code();
        }, color_);
    }

    std::string get_background_ansi_code() const {
        return std::visit([](auto&& it){
            return it.get_background_ansi_code();
        }, color_);
    }

    bool operator==(ansi_color other) const {
        return color_ == other.color_;
    }

private:
    std::variant<rgb_ansi_color, ansi_preset_color> color_;
};

class ansi_attribute {
public:
    enum its_kind {
        BOLD,
        DIM,
        ITALIC,
        UNDERLINE,
        BLINK,
        INVERSE,
        STRIKETHROUGH,
    };

    ansi_attribute(ansi_attribute::its_kind kind):
        kind_(kind) {
    }

    std::string get_ansi_code() const {
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

private:
    its_kind kind_;
};

struct ansi_formatting {
    std::optional<ansi_color> foreground_color = std::nullopt;
    std::optional<ansi_color> background_color = std::nullopt;
    std::optional<ansi_attribute> attribute = std::nullopt;

    std::string get_ansi_code() const {
        std::string ansi_code;

        if (foreground_color)
            ansi_code += foreground_color->get_foreground_ansi_code();

        if (background_color)
            ansi_code += background_color->get_background_ansi_code();

        if (attribute)
            ansi_code += attribute->get_ansi_code();

        return ansi_code;
    }
};

class colored_text_stream {
public:
    colored_text_stream():
        current_overlay_(std::nullopt) {
    }

    template <typename... format_args>
    void append(std::format_string<format_args...> fmt, format_args&&... args) {
        std::format_to(std::back_inserter(text_), fmt, std::forward<format_args>(args)...);

        if (!current_overlay_)
            return;

        current_overlay_->end = text_.size();
    }

    void clear_formatting() {
        if (current_overlay_)
            overlays_.push_back(*current_overlay_);
    }

    void set_formatting(ansi_formatting formatting) {
        clear_formatting();
        current_overlay_ = {formatting, text_.size(), text_.size()};
    }

    void set_foreground(ansi_color foreground) {
        set_formatting({ .foreground_color = foreground });
    }

    void set_background(ansi_color background) {
        set_formatting({ .background_color = background });
    }

    void set_attribute(ansi_attribute attribute) {
        set_formatting({ .attribute = attribute });
    }

    void print() {
        bool should_colorize = isatty(fileno(stdout));

        size_t open_index = 0, stop_index = 0;
        for (size_t i = 0; i < text_.size(); ++ i) {
            if (should_colorize) {
                bool should_end = false;
                while (stop_index < overlays_.size() && overlays_[stop_index].end >= i) {
                    should_end = true;
                    ++ stop_index;
                }

                if (should_end)
                    std::cout << RESET_SEQUENCE;

                while (open_index < overlays_.size() && overlays_[open_index].begin < i) {
                    std::cout << overlays_[open_index].formatting.get_ansi_code();
                    ++ open_index;
                }
            }

            std::cout << text_[i];
        }
    }

private:
    std::string text_;

    struct ansi_overlay {
        ansi_formatting formatting;
        size_t begin, end;
    };

    std::vector<ansi_overlay> overlays_;
    std::optional<ansi_overlay> current_overlay_;

    static inline constexpr std::string RESET_SEQUENCE = "\033[0m";
};

} // end namespace paracl
