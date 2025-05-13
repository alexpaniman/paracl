#pragma once

#include "paracl/structures/index-interval-treap.h"

#include <cstdint>
#include <variant>
#include <string>
#include <format>
#include <vector>
#include <cassert>


#ifdef _WIN32
#include <io.h>
#define isatty _isatty
#define fileno _fileno
#else
#include <unistd.h>
#endif

#include <print>


namespace paracl {

namespace {

constexpr std::string RESET_SEQUENCE = "\033[0m";

} // end anonymous namespace


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

        bool operator==(attribute other) const {
            return kind_ == other.kind_;
        }

    private:
        its_kind kind_;
    };

    struct formatting {
        std::optional<color> foreground_color = std::nullopt;
        std::optional<color> background_color = std::nullopt;
        std::optional<attribute> attribute = std::nullopt;

        std::string get_ansi_code() const;

        formatting& operator|=(const formatting &other) {
            if (other.foreground_color)
                this->foreground_color = other.foreground_color;

            if (other.background_color)
                this->background_color = other.background_color;

            if (other.attribute)
                this->attribute = other.attribute;

            return *this;
        }

        formatting operator|(const formatting &other) const {
            formatting copy = *this;
            copy |= other;
            return copy;
        }

        bool operator==(const formatting &other) const {
          return attribute == other.attribute &&
                 foreground_color == other.foreground_color &&
                 background_color == other.foreground_color;
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

    struct layered_formatting {
        formatting format;
        uint32_t layer;
    };

    void color(std::span<char> range, formatting format) {
        index_interval_tree<layered_formatting>::overlay range_overlay {
            static_cast<uint32_t>(range.begin() - range.begin()),
            static_cast<uint32_t>(range.end() - range.begin()),
            { format, current_layer_ ++ }
        };

        overlays_.insert(range_overlay);
    }

    void print(uint32_t begin, uint32_t end) {
        assert(end <= text_.size());

        const bool should_colorize = isatty(fileno(stdout));
        formatting previous = {}, current = {};

        std::vector<const index_interval_tree<layered_formatting>::overlay*> current_overlays;
        for (size_t i = begin; i < end; ++ i) {
            if (should_colorize) {
                current_overlays.clear();
                overlays_.query_point(i, current_overlays);

                std::sort(current_overlays.begin(), current_overlays.end(),
                          [](const auto *lhs, const auto *rhs) {
                              return lhs->data.layer >= rhs->data.layer;
                          });

                for (const auto *current_overlay: current_overlays)
                    current |= current_overlay->data.format;
            }

            if (previous != current) {
                std::print("{}", RESET_SEQUENCE);
                std::print("{}", current.get_ansi_code());
            }

            std::print("{}", text_[i]);
            previous = current;
        }
    }

    colored_text(std::span<char> text, index_interval_tree<layered_formatting> overlays, uint32_t current_layer): 
        text_(text),
        overlays_(overlays),
        current_layer_(current_layer) {
    }

    // colored_text substring(uint32_t begin, uint32_t end) {
    //     auto suboverlays = overlays_.substring(begin, end);
    //     auto subspan = text_.subspan(begin, end - begin);


    // } 

private:
    std::span<char> text_;

    index_interval_tree<layered_formatting> overlays_;
    uint32_t current_layer_ = 0;
};

} // end namespace paracl

