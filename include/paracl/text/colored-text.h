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

        formatting operator|(formatting other) {
            formatting copy = *this;

            if (!foreground_color)
                copy.foreground_color = other.foreground_color;

            if (!background_color)
                copy.background_color = other.background_color;

            if (!attribute)
                copy.attribute = other.attribute;

            return copy;
        }
    };

    struct fg {
        static inline formatting from_color(color foreground) {
            return {
                .foreground_color = foreground,
            };
        }

        static inline formatting BLACK          = from_color(color::BLACK);
        static inline formatting RED            = from_color(color::RED);
        static inline formatting GREEN          = from_color(color::GREEN);
        static inline formatting YELLOW         = from_color(color::YELLOW);
        static inline formatting BLUE           = from_color(color::BLUE);
        static inline formatting MAGENTA        = from_color(color::MAGENTA);
        static inline formatting CYAN           = from_color(color::CYAN);
        static inline formatting WHITE          = from_color(color::WHITE);
        static inline formatting BRIGHT_BLACK   = from_color(color::BRIGHT_BLACK);
        static inline formatting BRIGHT_RED     = from_color(color::BRIGHT_RED);
        static inline formatting BRIGHT_GREEN   = from_color(color::BRIGHT_GREEN);
        static inline formatting BRIGHT_YELLOW  = from_color(color::BRIGHT_YELLOW);
        static inline formatting BRIGHT_BLUE    = from_color(color::BRIGHT_BLUE);
        static inline formatting BRIGHT_MAGENTA = from_color(color::BRIGHT_MAGENTA);
        static inline formatting BRIGHT_CYAN    = from_color(color::BRIGHT_CYAN);
        static inline formatting BRIGHT_WHITE   = from_color(color::BRIGHT_WHITE);
    };

    struct bg {
        static inline formatting from_color(color background) {
            return {
                .background_color = background,
            };
        }

        static inline formatting BLACK          = from_color(color::BLACK);
        static inline formatting RED            = from_color(color::RED);
        static inline formatting GREEN          = from_color(color::GREEN);
        static inline formatting YELLOW         = from_color(color::YELLOW);
        static inline formatting BLUE           = from_color(color::BLUE);
        static inline formatting MAGENTA        = from_color(color::MAGENTA);
        static inline formatting CYAN           = from_color(color::CYAN);
        static inline formatting WHITE          = from_color(color::WHITE);
        static inline formatting BRIGHT_BLACK   = from_color(color::BRIGHT_BLACK);
        static inline formatting BRIGHT_RED     = from_color(color::BRIGHT_RED);
        static inline formatting BRIGHT_GREEN   = from_color(color::BRIGHT_GREEN);
        static inline formatting BRIGHT_YELLOW  = from_color(color::BRIGHT_YELLOW);
        static inline formatting BRIGHT_BLUE    = from_color(color::BRIGHT_BLUE);
        static inline formatting BRIGHT_MAGENTA = from_color(color::BRIGHT_MAGENTA);
        static inline formatting BRIGHT_CYAN    = from_color(color::BRIGHT_CYAN);
        static inline formatting BRIGHT_WHITE   = from_color(color::BRIGHT_WHITE);
    };

    static inline formatting BOLD = {
        .foreground_color = std::nullopt,
        .background_color = std::nullopt,
        .attribute = attribute::BOLD
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

    void print() const;


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
