#pragma once
#include <concepts>


namespace paracl {

template <std::integral type>
struct index_span {
    type begin, end;

    bool contains(type point) { return begin <= point && point < end; }
};
    
} // end namespace paracl

