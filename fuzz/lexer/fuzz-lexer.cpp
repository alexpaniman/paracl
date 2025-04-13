#include <cassert>
#include <cstddef>
#include <cstdint>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    assert(data != nullptr);
    assert(size != 0);

    return 0;
}

