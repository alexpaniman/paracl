#pragma once

#include <cstdint>


namespace paracl {

/*
Node's execute function must return either a value, a pointer to a value, or nothing.
Since we need pointers to int, the last bit of the pointer is zero, so we'll use this
*/

enum encoding_flag {
    VALUE   = 0b0,
    POINTER = 0b1,
};

inline int64_t create_pointer(int64_t *value) {
    return reinterpret_cast<int64_t>(value) | POINTER;
}

inline int64_t create_value(int64_t value) {
    return ((value << 1) | VALUE);
}

inline int64_t *get_pointer(int64_t value) {
    if ((value & 0b1) == VALUE) {
        //обработка
    }

    return reinterpret_cast<int64_t*>(value & ~POINTER);
}

inline int64_t get_value(int64_t value) {
    if ((value & 0b1) == POINTER) {
        return *(reinterpret_cast<int64_t*>(value & ~POINTER));
    }

    return (value >> 1);
}

} // end namespace paracl