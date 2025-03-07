#pragma once

#include "paracl/text/display.h"
#include "paracl/text/colored-text.h"

#include <span>
#include <cassert>


namespace paracl {

/// Anatomy of message arrow:
///
///  underline_connection  > ^~~~~~
///  upwards_lines         > | ^
///                          | underline
///                          |
///                          +-------
///   leftwards_upwards_join ^^
///                           leftwards_line
struct annotation_arrow_config {
    const char *underline_connection;
    const char *underline;
    const char *upwards_line;
    const char *leftwards_upwards_join;
    const char *leftwards_line;
};

///  underline_connection  > ^~~~~~
///  upwards_lines         > | ^
///                          | underline
///                          |
///                          +------- <message>
///   leftwards_upwards_join ^^
///                           leftwards_line
inline constexpr annotation_arrow_config clang_arrow = {
    .underline_connection = "^",
    .underline = "~",
    .upwards_line = "|",
    .leftwards_upwards_join = "+",
    .leftwards_line = "-",
};

///  underline_connection  > ┬─────
///  upwards_lines         > │ ^
///                          │ underline
///                          │
///                          └─────── <message>
///   leftwards_upwards_join ^^
///                           leftwards_line
inline constexpr annotation_arrow_config box_arrow = {
    .underline_connection = "┬",
    .underline = "─",
    .upwards_line = "│",
    .leftwards_upwards_join = "└",
    .leftwards_line = "─",
};

struct annotation_config {
    colored_text::formatting line_format;
    annotation_arrow_config arrow_config;
    int32_t line_numbers_space;
};

inline annotation_config default_annotation_config = {
    .line_format = colored_text::fg::CYAN | colored_text::BOLD,
    .arrow_config = box_arrow,
    .line_numbers_space = 5,
};


colored_text annotate(std::span<char> text, std::vector<annotated_range> ranges,
                      annotation_config config = default_annotation_config);

} // end namespace paracl
