#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <span>


namespace paracl {

struct text_position {
    uint32_t point;
    uint32_t line, column;

    auto operator<=>(const text_position &other) const {
        return point <=> other.point;
    }
};

struct text_range {
    text_position x0, x1;

    auto operator<=>(const text_range &other) const {
        return x0 <=> other.x0;
    }
};

struct file_position {
    std::string filename;
    text_position position;
};

struct file_range {
    std::string filename;
    text_range range;
};


struct file {
    std::string filename;
    std::string text;
};

void print_range(std::span<char> text, std::vector<text_range> ranges);

} // end namespace paracl

