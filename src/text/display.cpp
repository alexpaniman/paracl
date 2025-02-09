#include "paracl/text/display.h"

#include <cstdint>
#include <span>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <iomanip>


namespace paracl {

namespace {

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

bool print_annotations(int32_t start, int32_t end, std::span<text_range> ranges, bool dry_run = false) {
    bool has_annotations = false;

    for (int32_t i = start; i < end; ++ i) {
        bool beginning = false;
        bool inside = false;
        for (size_t j = 0; j < ranges.size(); ++ j) {
            if (i <  static_cast<int32_t>(ranges[j].begin.point))
                continue;

            if (i == static_cast<int32_t>(ranges[j].begin.point)) {
                inside = true;
                beginning = true;
                continue;
            }

            if (i <  static_cast<int32_t>(ranges[j].end.point))
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

} // end anonymous namespace


void print_range(std::span<char> text, std::vector<text_range> ranges) {
    static constexpr int32_t line_numbers_space = 5;

    assert(!ranges.empty());
    std::sort(ranges.begin(), ranges.end());

    auto &position_open = ranges[0].begin;
    auto &position_stop = ranges[ranges.size() - 1].end;

    // compute the beginning line:
    int32_t i = position_open.point - position_open.column;

    int32_t line_min = position_open.line;
    int32_t line_max = get_previous_iter_line(position_stop);

    for (int32_t line = line_min; line <= line_max; ++ line) {
        std::cout << std::setw(line_numbers_space) << line << " | ";

        int32_t start = i;
        print_line(text, i);

        int32_t end = i;


        bool has_annotations =
            print_annotations(start, end, ranges, /*dry_run=*/true);

        if (has_annotations) {
            std::cout << std::setw(line_numbers_space) << "" << " | ";
            print_annotations(start, end, ranges, /*dry_run=*/false);
        }
    }
}

} // end namespace paracl

