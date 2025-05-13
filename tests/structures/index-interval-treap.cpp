#include "paracl/structures/index-interval-treap.h"
#include "catch2/catch2.h"

/*

TEST_CASE("Interval tree basic insert and range query") {
    index_interval_tree<int> tree;
    tree.insert({0, 5, 1});
    tree.insert({3, 8, 2});
    tree.insert({7, 10, 3});

    auto result = tree.query_range(4, 9);
    REQUIRE(result.size() == 3);
}

TEST_CASE("Query point inside interval") {
    index_interval_tree<int> tree;
    tree.insert({10, 20, 5});
    auto result = tree.query_point(15);
    REQUIRE(result.size() == 1);
    REQUIRE(result[0]->data == 5);
}

TEST_CASE("Query point outside interval") {
    index_interval_tree<int> tree;
    tree.insert({10, 20, 5});
    auto result = tree.query_point(25);
    REQUIRE(result.empty());
}

TEST_CASE("Subtree query correctness") {
    index_interval_tree<int> tree;
    tree.insert({0, 5, 1});
    tree.insert({4, 8, 2});
    tree.insert({7, 10, 3});

    auto sub_tree = tree.substring(3, 9);
    auto result = sub_tree.query_range(0, 6);
    REQUIRE(result.size() == 3);
    REQUIRE(result[0]->data == 1);
    REQUIRE(result[1]->data == 2);
    REQUIRE(result[2]->data == 3);
}

TEST_CASE("No overlap case") {
    index_interval_tree<int> tree;
    tree.insert({0, 5, 1});
    auto result = tree.query_range(6, 8);
    REQUIRE(result.empty());
}

TEST_CASE("Multiple overlapping inserts") {
    index_interval_tree<int> tree;
    for (uint32_t i = 0; i < 100; ++i)
        tree.insert({i, i + 10, static_cast<int>(i)});
    auto result = tree.query_range(50, 60);
    REQUIRE(result.size() > 0);
    for (auto *ov : result) {
        REQUIRE(ov->begin < 60);
        REQUIRE(ov->end > 50);
    }
}



*/
