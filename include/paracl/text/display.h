#pragma once

#include <string>
#include <vector>
#include <span>
#include <cassert>


namespace paracl {

struct text_position {
    size_t point;
    size_t line, column;

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

struct file {
    std::string filename;
    std::string text;
};

struct annotated_range {
    text_range range;
    std::string annotation;

    auto operator<=>(const annotated_range &other) const {
        return range <=> other.range;
    }
}; 

void print_range(std::span<char> text, std::vector<annotated_range> ranges);

} // end namespace paracl

