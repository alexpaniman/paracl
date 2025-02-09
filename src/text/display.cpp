#include "paracl/text/display.h"

#include <span>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <iomanip>


namespace paracl {

void print_range(std::span<char> text, std::vector<text_range> ranges) {

    assert(!ranges.empty());
    std::sort(ranges.begin(), ranges.end());

    // compute the beginning of the first line:
    int32_t i = ranges[0].x0.point;
    while (text[i] != '\n' && i > 0)
        -- i;

    if (i != 0)
        ++ i;

    int32_t line_from = ranges[0].x0.line;
    int32_t line_to   = ranges[ranges.size() - 1].x1.line - (ranges[ranges.size() - 1].x1.column == 0 ? 1 : 0);

    for (int32_t line = line_from; line <= line_to; ++ line) {
        std::cout << std::setw(5) << line << " | ";

        // == print a line

        int32_t start = i;

        for (;; ++ i) {
            if (i > (int32_t) text.size())
                break;

            std::cout << text[i];

            if (text[i] == '\n')
                break;
        }

        if (text[i] == '\n')
            ++ i;

        if (i > (int32_t) text.size()) {
            std::cout << "\n"; // TODO: ??
        }

        // ===============

        std::cout << std::setw(5) << "" << " | ";

        i = start;
        for (;; ++ i) {
            if (i > (int32_t) text.size())
                break;

            bool beginning = false;
            bool inside = false;
            for (uint32_t current_range = 0; current_range < ranges.size(); ++ current_range) {
                if (i <  (int32_t) ranges[current_range].x0.point)
                    ;
                else
                if (i == (int32_t) ranges[current_range].x0.point)
                    inside = beginning = true;
                else
                if (i <  (int32_t) ranges[current_range].x1.point)
                    inside = true;
            }

            if (!inside)
                std::cout << " ";
            else
            if (beginning)
                std::cout << "^";
            else
                std::cout << "~";


            if (text[i] == '\n') {
                std::cout << "\n";
                break;
            }
        }

        if (text[i] == '\n')
            ++ i;

        if (i > (int32_t) text.size()) {
            std::cout << "\n"; // TODO: ??
            break;
        }
    }
}

} // end namespace paracl

