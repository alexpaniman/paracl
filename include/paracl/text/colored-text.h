#pragma once

#include <cstdint>
#include <variant>
#include <string>
#include <format>
#include <vector>


namespace paracl {

class colored_text {
public:
    class color {
    public:
        enum preset_kind {
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

        color(color::preset_kind kind):
            color_(color::preset(kind)) {
        }

        color(uint8_t r, uint8_t g, uint8_t b):
            color_(color::rgb(r, g, b)) {
        }

        std::string get_foreground_ansi_code() const;
        std::string get_background_ansi_code() const;

        bool operator==(color other) const;

    private:
        class preset {
        public:
            preset(color::preset_kind kind):
                kind_(kind) {
            }

            std::string get_foreground_ansi_code() const;
            std::string get_background_ansi_code() const;

            bool operator==(color::preset other) const;

        private:
            preset_kind kind_;
        };

        class rgb {
        public:
            rgb(uint8_t r, uint8_t g, uint8_t b):
                r_(r), g_(g), b_(b) {
            }

            std::string get_foreground_ansi_code() const;
            std::string get_background_ansi_code() const;

            bool operator==(color::rgb other) const;

        private:
            uint8_t r_, g_, b_;
        };

        std::variant<color::rgb, color::preset> color_;
    };

    class attribute {
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

        attribute(attribute::its_kind kind):
            kind_(kind) {
        }

        std::string get_ansi_code() const;

    private:
        its_kind kind_;
    };

    struct formatting {
        std::optional<color> foreground_color = std::nullopt;
        std::optional<color> background_color = std::nullopt;
        std::optional<attribute> attribute = std::nullopt;

        std::string get_ansi_code() const;
    };

    colored_text():
        current_overlay_(std::nullopt) {
    }

    template <typename... format_args>
    void append(std::format_string<format_args...> fmt, format_args&&... args) {
        std::format_to(
            std::back_inserter(text_),
            fmt, std::forward<format_args>(args)...
        );
    }

    void clear_formatting();
    void set_formatting(formatting formatting);

    void set_foreground(color foreground);
    void set_background(color background);
    void set_attribute(attribute attribute);

    void print();


private:
    std::string text_;

    struct formatting_overlay {
        formatting formatting;
        size_t begin, end;
    };

    std::vector<formatting_overlay> overlays_;
    std::optional<formatting_overlay> current_overlay_;
};

} // end namespace paracl

