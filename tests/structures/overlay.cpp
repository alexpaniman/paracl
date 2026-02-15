#include "catch2/catch2.h"
#include "paracl/structures/overlay.h"

#include <cstdint>
#include <stdint.h>


// Helper to create overlay
static paracl::text_overlays<uint32_t>::overlay ov(uint32_t b, uint32_t e, uint32_t v) {
    return paracl::text_overlays<uint32_t>::overlay{ {b, e}, v };
}

TEST_CASE("get_overlay_at_point returns correct overlay or nullopt", "[text_overlays]") {
    std::vector<paracl::text_overlays<uint32_t>::overlay> vec = {
      ov(0, 5, 1),
      ov(10, 15, 2)
    };

    paracl::text_overlays<uint32_t> tos{vec};

    auto o1 = tos.get_overlay_at_point(3);
    REQUIRE(o1.has_value());
    CHECK(o1->span.begin == 0);
    CHECK(o1->span.end == 5);
    CHECK(o1->value == 1);

    auto o2 = tos.get_overlay_at_point(5);
    REQUIRE(!o2.has_value());

    auto o3 = tos.get_overlay_at_point(12);
    REQUIRE(o3.has_value());
    CHECK(o3->value == 2);
}

TEST_CASE("subspan preserves and adjusts overlays inside span", "[text_overlays]") {
    std::vector<paracl::text_overlays<uint32_t>::overlay> vec = { ov(0, 5, 1), ov(5, 10, 2), ov(10, 20, 3) };
    paracl::text_overlays<uint32_t> tos{ vec };

    auto sub = tos.subspan({5, 15});
    // inside [5,15) should include overlays [5,10) and [10,15)
    std::vector<paracl::text_overlays<uint32_t>::overlay> expected = { ov(0, 5, 2), ov(5, 10, 3) };
    for (auto &exp : expected) {
        auto o = sub.get_overlay_at_point(exp.span.begin);
        REQUIRE(o.has_value());
        CHECK(o->span == exp.span);
        CHECK(o->value == exp.value);
    }
    // outside should not exist
    REQUIRE(!sub.get_overlay_at_point(15).has_value());
}

TEST_CASE("inject_overlays inserts non-overlapping overlays into empty", "[text_overlays]") {
    paracl::text_overlays<uint32_t> tos{ std::vector<paracl::text_overlays<uint32_t>::overlay>{} };
    std::vector<paracl::text_overlays<uint32_t>::overlay> to_inject = { ov(0, 5, 1), ov(10, 15, 2) };
    tos.inject_overlays(to_inject.begin(), to_inject.end());

    auto o1 = tos.get_overlay_at_point(2);
    REQUIRE(o1.has_value());
    CHECK(o1->value == 1);
    auto o2 = tos.get_overlay_at_point(12);
    REQUIRE(o2.has_value());
    CHECK(o2->value == 2);
}

TEST_CASE("inject_overlays replaces overlapping overlays", "[text_overlays]") {
    std::vector<paracl::text_overlays<uint32_t>::overlay> base = { ov(0, 10, 1), ov(20, 30, 2) };
    paracl::text_overlays<uint32_t> tos{ base };
    std::vector<paracl::text_overlays<uint32_t>::overlay> to_inject = { ov(5, 25, 9) };
    tos.inject_overlays(to_inject.begin(), to_inject.end());

    // [0,5) from original
    auto o0 = tos.get_overlay_at_point(2);
    REQUIRE(o0.has_value());
    CHECK(o0->span.begin == 0);
    CHECK(o0->span.end == 5);
    // injected
    auto oi = tos.get_overlay_at_point(10);
    REQUIRE(oi.has_value());
    CHECK(oi->span.begin == 5);
    CHECK(oi->span.end == 25);
    CHECK(oi->value == 9);
    // [25,30)
    auto o2 = tos.get_overlay_at_point(28);
    REQUIRE(o2.has_value());
    CHECK(o2->span.begin == 25);
    CHECK(o2->span.end == 30);
    CHECK(o2->value == 2);
}

