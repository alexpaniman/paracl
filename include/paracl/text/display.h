#pragma once

#include "paracl/text/colors.h"
#include <iostream>
#include <string>
#include <vector>
#include <span>
#include <cassert>


namespace paracl {

struct text_position {
    size_t point;
    size_t line, column;

    text_position() = default;

    text_position(size_t point, size_t line, size_t column):
        point(point),
        line(line),
        column(column) {

        assert(point >= (line - 1) + column);
    }

    auto operator<=>(const text_position &other) const {
        return point <=> other.point;
    }

    static text_position from_point(std::span<char> text, int32_t point) {
        assert(point >= 0);
        assert(!text.empty());

        int32_t saved_point = point;

        int32_t column = 0;
        if (text[point] == '\n' && point > 0)
            ++ column;

        while (text[point] != '\n' && point > 0)
            ++ column;

        if (point != 0)
            -- column;

        point -= column;

        int32_t line = 1;
        for (; point >= 0; -- point)
            if (text[point] == '\n')
                ++ line;

        return text_position(saved_point, line, column);
    }
};

struct text_range {
    text_position begin, end;

    text_range() = default;

    text_range(text_position begin, text_position end):
        begin(begin),
        end(end) {

        assert(begin.point <= end.point);
    }

    text_range(std::span<char> text, int32_t begin_point, int32_t end_point):
        begin(text_position::from_point(text, begin_point)),
        end(text_position::from_point(text, end_point)) {

        assert(begin_point <= end_point);
    }

    auto operator<=>(const text_range &other) const {
        return begin <=> other.begin;
    }
};

struct annotated_range {
    text_range range;
    std::string annotation;

    auto operator<=>(const annotated_range &other) const {
        return range <=> other.range;
    }
}; 

struct annotation_config {
    ansi_formatting line = {};
    ansi_formatting file = {};
    ansi_formatting nums = {};
    size_t show_before = 0, show_after = 0;
};


void print_range(colored_text_stream &stream,
                 std::span<char> text,
                 std::vector<annotated_range> ranges,
                 annotation_config cfg);



template <typename type>
struct rngable {};

struct rng {
    rng() = default;

    template <typename type>
    rng(const type &location) {
        actual_range.range = rngable<type>::to_range(location);
    }

    template <typename type>
    rng(const type &location, const std::string &text) {
        actual_range.range = rngable<type>::to_range(location);
        actual_range.annotation = text;
    }

    template <typename type>
    rng(const type &x0, const type &x1) {
        text_range range0 = rngable<type>::to_range(x0);
        text_range range1 = rngable<type>::to_range(x1);

        actual_range.range.begin = range0.begin;
        actual_range.range.end = range1.end;
    }

    template <typename type>
    rng(const type &x0, const type &x1, const std::string &text) {
        text_range range0 = rngable<type>::to_range(x0);
        text_range range1 = rngable<type>::to_range(x1);

        actual_range.range.begin = range0.begin;
        actual_range.range.end = range1.end;

        actual_range.annotation = text;
    }

    annotated_range actual_range;
};

struct file {
    std::string filename;
    std::string text;

    void message(const std::string &message, std::vector<rng> rngs) {
        assert(rngs.size() != 0);

        std::vector<annotated_range> ranges;
        for (auto rng: rngs)
            ranges.push_back(rng.actual_range);

        std::sort(ranges.begin(), ranges.end());

        std::cout << filename << ":" << ranges[0].range.begin.line << ":" << ranges[0].range.begin.column << ": " << message << "\n";

        annotation_config cfg {
            .line = { .foreground_color = ansi_preset_color::GREEN }
        };

        colored_text_stream stream;
        print_range(stream, text, ranges, cfg);

        stream.print();
    }
};


} // end namespace paracl

