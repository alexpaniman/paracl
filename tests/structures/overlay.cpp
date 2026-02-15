#include "catch2/catch2.h"
#include "paracl/structures/overlay.h"

#include <cstdint>
#include <stdint.h>
#include <cassert>
#include <iostream>


// Helper to create overlay
static paracl::text_overlays<uint32_t>::overlay ov(uint32_t b, uint32_t e, uint32_t v) {
    return paracl::text_overlays<uint32_t>::overlay{ {b, e}, v };
}



// for the simplicity of tests, only overlays with u32 values are tested:
using overlays = paracl::text_overlays<uint32_t>;
using index_span = paracl::index_span<uint32_t>;


bool has_overlay_numbered(const overlays &self, uint32_t value) {
    for (const auto &overlay: self.get_overlays())
        if (overlay.value == value)
            return true;

    return false;
}

uint32_t get_overlays_end(const overlays &self) {
    uint32_t end = 0;
    for (const auto &overlay: self.get_overlays())
        end = std::max<uint32_t>(end, overlay.span.end);      

    return static_cast<int64_t>(end);
}


// actual representation of overlays:
// xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx -- underlying text
// ________________________ _____ _________ _____
// 9                        1     7         2


// less efficient, but easier to verify representation:

// xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx -- underlying text
// 9999999999999999999999990111110777777777022222


// convert overlays from their actual representation to easier to verify one,
// empty spaces are filled with zeros for simplicity, so don't pass overlays
// that have a span numbered zero to avoid collisions:
std::vector<uint32_t> lay_overlays(const overlays &self) {
    assert(!has_overlay_numbered(self, 0) && "zero is used to indicate spaces");

    uint32_t end = get_overlays_end(self);
    std::vector<uint32_t> superimposed_overlays(end, static_cast<uint32_t>(0));

    auto overlays = self.get_overlays();
    for (const auto &overlay: overlays)
        for (uint32_t i = overlay.span.begin; i < overlay.span.end; ++ i) {
            // spans should not overlap
            REQUIRE(superimposed_overlays[i] == 0);
            superimposed_overlays[i] = overlay.value;
        }

    return superimposed_overlays;
}

void verify_get_overlay_at_point(const overlays &self) {
    uint32_t end = get_overlays_end(self);
    std::vector<bool> has_overlay(end, false);

    for (const auto &ov : self.get_overlays())
        for (uint32_t i = ov.span.begin; i < ov.span.end; ++ i) {
            has_overlay[i] = true;

            auto found = self.get_overlay_at_point(i);
            REQUIRE(found);

            // compare in separate REQUIREs for easier debugging

            REQUIRE(found.value().span.begin == ov.span.begin);
            REQUIRE(found.value().span.end == ov.span.end);

            REQUIRE(found.value().value == ov.value);
        }

    // ensure in each place where overlay is absent:
    for (size_t i = 0; i < end; ++ i)
        REQUIRE(has_overlay[i] == !!self.get_overlay_at_point(i));
}

void verify_subspan(overlays &self, index_span span) {
    auto subspan = self.subspan(span);
    auto subspan_projected = lay_overlays(subspan);
    auto overlay_projected = lay_overlays(self);

    std::cout << "span: " << span.begin << " " << span.end << "\n";

    std::cout << "subspan: ";
    for (uint32_t i = 0; i < subspan_projected.size(); ++ i)
        std::cout << subspan_projected[i] << "";
    std::cout << "\n";

    std::cout << "project: ";
    for (uint32_t i = 0; i < overlay_projected.size(); ++ i)
        std::cout << overlay_projected[i] << "";
    std::cout << "\n";

    for (uint32_t i = 0; i < subspan_projected.size(); ++i) {
        uint32_t superimposed_i = i + span.begin;
        REQUIRE(overlay_projected[superimposed_i] == subspan_projected[i]);
    }
}

void verify_subspans(overlays &self) {
    uint32_t upper_bound = get_overlays_end(self);

    for (uint32_t end = 1; end <= upper_bound; ++ end)
        for (uint32_t begin = 0; begin < end; ++ begin)
            verify_subspan(self, {begin, end});
}

TEST_CASE("get_overlay_at_point returns correct overlay or nullopt", "[text_overlays]") {
    std::vector<paracl::text_overlays<uint32_t>::overlay> vec = {
        ov(0, 5, 1),
        ov(10, 15, 2)
    };

    std::string message = "hello world hello world hello world";

    paracl::text_overlays<uint32_t> tos{vec, message};

    verify_get_overlay_at_point(tos);
}

TEST_CASE("subspan returns correct set of overlays for every two points", "[text_overlays]") {
    std::vector<paracl::text_overlays<uint32_t>::overlay> vec = {
        ov(0, 5, 1),
        ov(10, 15, 2)
    };

    std::string message = "hello world hello world hello world";

    overlays tos{vec, message};
    verify_subspans(tos);
}


