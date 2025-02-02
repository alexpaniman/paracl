#pragma once

#include <string>


namespace paracl {

struct text_position {
    int point;
    int line, column;
};

struct text_range {
    text_position x0, x1;
};

struct file_position {
    std::string filename;
    text_position position;
};

struct file_range {
    std::string filename;
    text_range range;
};

void print_annotated_range(const file_range &range, const std::string &annot);

} // end namespace paracl

