#include "paracl/text/display.h"

#include <cstdint>
#include <span>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <iomanip>


namespace paracl {

namespace {

struct annotated_point {
    text_position position;
    std::string annotation;

    auto operator<=>(const annotated_point &other) const {
        return position <=> other.position;
    }
}; 

int32_t get_previous_iter_line(text_position pos) {
    return pos.line - (pos.column == 0 ? 1 : 0);
}

void print_line(std::span<char> text, int32_t &i) {
    int32_t size = text.size();

    while (true) {
        if (i > size)
            break;

        std::cout << text[i];

        if (text[i] == '\n')
            break;

        ++ i;
    }

    if (text[i] == '\n')
        ++ i;

    if (i > size)
        std::cout << "\n";
}

bool print_annotations(size_t start, size_t end, std::span<annotated_range> ranges, bool dry_run = false) {
    bool has_annotations = false;

    for (size_t i = start; i < end; ++ i) {
        bool beginning = false;
        bool inside = false;
        for (size_t j = 0; j < ranges.size(); ++ j) {
            if (i <  ranges[j].range.begin.point)
                continue;

            if (i == ranges[j].range.begin.point) {
                inside = true;
                beginning = true;
                continue;
            }

            if (i <  ranges[j].range.end.point)
                inside = true;
        }

        if (!dry_run) {
            if (!inside)
                std::cout << " ";
            else
            if (beginning)
                std::cout << "^";
            else
                std::cout << "~";
        }

        has_annotations |= inside;
    }

    if (!dry_run)
        std::cout << "\n";

    return has_annotations;
}

auto get_join_points(size_t begin, size_t end, std::vector<annotated_range> ranges) {
    std::vector<annotated_point> line_ranges;
    for (size_t j = 0; j < ranges.size(); ++ j) {
        if (ranges[j].range.end.point >= end)
            continue;

        if (ranges[j].range.end.point < begin)
            continue;

        if (ranges[j].range.begin.line != ranges[j].range.end.line) {
            line_ranges.push_back({ranges[j].range.end, ranges[j].annotation});
        } else {
            line_ranges.push_back({ranges[j].range.begin, ranges[j].annotation});
        }
    }

    return line_ranges;
}

void print_line_number(int32_t line_numbers_space, auto line) {
    std::cout << std::setw(line_numbers_space) << line << " | ";
}

void print_messages(std::vector<annotated_point> ranges) {
    std::sort(ranges.begin(), ranges.end());

    print_line_number(5, "");

    {
        size_t current_column = 0;
        for (unsigned i = 0; i < ranges.size(); ++ i) {
            auto &[pos, annotation] = ranges[i];
            while (current_column < pos.column) {
                std::cout << " ";
                ++ current_column;
            }

            std::cout << "|";
        }

        std::cout << "\n";
    }

    for (int j = ranges.size() - 1; j >= 0; -- j) {
        print_line_number(5, "");

        size_t current_column = 1;
        for (int i = 0; i <= j; ++ i) {
            auto &[pos, annotation] = ranges[i];

            while (current_column < pos.column) {
                std::cout << " ";
                ++ current_column;
            }

            if (i == j)
                std::cout << annotation;
            else
                std::cout << "|";
        }

        std::cout << "\n";
    }
}

} // end anonymous namespace


void print_range(std::span<char> text, std::vector<annotated_range> ranges) {
    static constexpr int32_t line_numbers_space = 5;

    assert(!ranges.empty());
    std::sort(ranges.begin(), ranges.end());

    auto &position_open = ranges[0].range.begin;
    auto &position_stop = ranges[ranges.size() - 1].range.end;

    // compute the beginning line:
    int32_t i = position_open.point - position_open.column;

    int32_t line_min = position_open.line;
    int32_t line_max = get_previous_iter_line(position_stop);

    for (int32_t line = line_min; line <= line_max; ++ line) {
        print_line_number(line_numbers_space, line);

        int32_t start = i;
        print_line(text, i);

        int32_t end = i;

        bool has_annotations =
            print_annotations(start, end, ranges, /*dry_run=*/true);

        if (has_annotations) {
            print_line_number(line_numbers_space, "");
            print_annotations(start, end, ranges, /*dry_run=*/false);

            auto line_ranges = get_join_points(start, end, ranges);
            print_messages(std::move(line_ranges));
        }
    }
}

} // end namespace paracl

