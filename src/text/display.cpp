#include "paracl/text/display.h"
#include "paracl/text/ansi.h"

#include <cstdint>
#include <span>
#include <cassert>
#include <algorithm>


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

void print_line(colored_text_stream stream, std::span<char> text, int32_t &i) {
    int32_t size = text.size();

    while (true) {
        if (i > size)
            break;

        stream.append("{}", text[i]);

        if (text[i] == '\n')
            break;

        ++ i;
    }

    if (text[i] == '\n')
        ++ i;

    if (i > size)
        stream.append("\n");
}

bool print_annotations(colored_text_stream &stream,
                       size_t start, size_t end,
                       std::span<annotated_range> ranges,
                       annotation_config cfg,
                       bool dry_run = false) {

    stream.set_formatting(cfg.line);

    bool has_annotations = false;
    for (size_t i = start; i < end; ++ i) {
        bool beginning = false;
        bool inside = false;
        bool should_connect = false;

        for (size_t j = 0; j < ranges.size(); ++ j) {
            if (i <  ranges[j].range.begin.point)
                continue;

            if (i == ranges[j].range.begin.point) {
                inside = true;
                beginning = true;

                should_connect |= !ranges[j].annotation.empty();
                continue;
            }

            if (i <  ranges[j].range.end.point)
                inside = true;
        }

        if (!dry_run) {
            if (!inside)
                stream.append(" ");
            else
            if (beginning && should_connect)
                stream.append("┬");
            else {
                stream.append("─");
            }
        }

        has_annotations |= inside;
    }

    if (!dry_run) {
        stream.clear_formatting();
        stream.append("\n");
    }

    return has_annotations;
}

auto get_join_points(std::vector<annotated_range> ranges, size_t line) {
    std::vector<annotated_point> line_ranges;
    for (size_t j = 0; j < ranges.size(); ++ j) {
        if (ranges[j].range.begin.line != line)
            continue;

        if (ranges[j].annotation.empty())
            continue;

        line_ranges.push_back({ranges[j].range.begin, ranges[j].annotation});
    }

    return line_ranges;
}

void print_line_number(colored_text_stream &stream,
                       int32_t line_numbers_space, auto line) {

    (void) line_numbers_space;
    stream.append("{:>5} | ", line);
}

void print_messages(colored_text_stream &stream,
                    std::vector<annotated_point> ranges, size_t column_alignment = 0) {

    std::sort(ranges.begin(), ranges.end());

    size_t max_column = column_alignment;
    for (int j = ranges.size() - 1; j >= 0; -- j) {
        print_line_number(stream, 5, "");

        stream.append("{}", GREEN);

        size_t current_column = 0;
        for (int i = 0; i <= j; ++ i) {
            auto &[pos, annotation] = ranges[i];

            while (current_column < pos.column) {
                stream.append(" ");
                ++ current_column;
            }

            if (i == j) {
                if (current_column > max_column)
                    max_column = current_column;

                if (current_column < max_column) {
                    stream.append("└");
                    ++ current_column;
                }

                while (current_column < max_column) {
                    stream.append("─");
                    ++ current_column;
                }

                stream.append(" {}", annotation);

                current_column += annotation.size() + 1;
            } else {
                stream.append("│");
                ++ current_column;
            }
        }

        stream.clear_formatting();

        std::cout << "\n";
    }

    print_line_number(stream, 5, "");
    stream.append("\n");
}

} // end anonymous namespace


void print_range(colored_text_stream stream,
                 std::span<char> text,
                 std::vector<annotated_range> ranges,
                 annotation_config cfg) {

    static constexpr int32_t line_numbers_space = 5;

    assert(!ranges.empty());
    std::sort(ranges.begin(), ranges.end());

    auto &position_open = ranges[0].range.begin;
    auto &position_stop = ranges[ranges.size() - 1].range.end;

    // compute the beginning line:
    int32_t i = position_open.point - position_open.column;

    int32_t line_min = position_open.line;
    int32_t line_max = get_previous_iter_line(position_stop);

    // =============== column
    size_t max_column = 0;
    for (auto &&range: ranges) {
        if (max_column < range.range.begin.column)
            max_column = range.range.begin.column;
    }

    max_column += 2;
    // ======================

    for (int32_t line = line_min; line <= line_max; ++ line) {
        print_line_number(stream, line_numbers_space, line);

        int32_t start = i;
        print_line(stream, text, i);

        int32_t end = i;

        bool has_annotations =
            print_annotations(stream, start, end, ranges, cfg, /*dry_run=*/true);

        if (has_annotations) {
            print_line_number(stream, line_numbers_space, "");
            print_annotations(stream, start, end, ranges, cfg, /*dry_run=*/false);

            auto line_ranges = get_join_points(ranges, line);
            if (!line_ranges.empty())
                print_messages(stream, std::move(line_ranges), max_column);
        }
    }
}

} // end namespace paracl

