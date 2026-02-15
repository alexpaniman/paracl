#pragma once
#include <concepts>


namespace paracl {

template <std::integral type>
struct index_span {
    type begin, end;

    bool contains(type point) const { return begin <= point && point < end; }
    bool contains(index_span other) const { return begin <= other.begin && end >= other.end; }

    bool operator==(const index_span &other) const {
        return begin == other.begin && end == other.end;
    }
};
    
} // end namespace paracl

