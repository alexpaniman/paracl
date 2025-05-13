#include <vector>
#include <algorithm>
#include <random>
#include <cassert>
#include <vector>
#include <algorithm>
#include <random>
#include <cstdint>

#include "overlay.h"


template <typename value_type>
struct index_interval_tree {
public:
    struct overlay {
        uint32_t begin;
        uint32_t end;
        value_type data;
    };

private:
    struct node {
        overlay interval;

        uint32_t max_end;
        int32_t left = -1, right = -1;
        uint32_t priority;
    };

public:
    uint32_t random_priority() { return rng(); }

    void update_node(int idx) {
        auto &n = nodes[idx];
        n.max_end = n.interval.end;

        if (n.left != -1)
            n.max_end = std::max(n.max_end, nodes[n.left].max_end);

        if (n.right != -1)
            n.max_end = std::max(n.max_end, nodes[n.right].max_end);
    }

    int rotate_right(int y) {
        int x = nodes[y].left;
        nodes[y].left = nodes[x].right;
        nodes[x].right = y;

        update_node(y);
        update_node(x);
        return x;
    }

    int rotate_left(int x) {
        int y = nodes[x].right;
        nodes[x].right = nodes[y].left;
        nodes[y].left = x;

        update_node(x);
        update_node(y);
        return y;
    }

    int insert_node(int idx, const overlay &ov) {
        if (idx == -1) {
            nodes.push_back({ov, ov.end, -1, -1, random_priority()});
            return static_cast<int>(nodes.size() - 1);
        }
        if (ov.begin < nodes[idx].interval.begin) {
            nodes[idx].left = insert_node(nodes[idx].left, ov);
            if (nodes[nodes[idx].left].priority < nodes[idx].priority)
                idx = rotate_right(idx);
        } else {
            nodes[idx].right = insert_node(nodes[idx].right, ov);
            if (nodes[nodes[idx].right].priority < nodes[idx].priority)
                idx = rotate_left(idx);
        }

        update_node(idx);
        return idx;
    }

    void insert(const overlay &ov) {
        root = insert_node(root, ov);
    }

    void query_range(uint32_t begin, uint32_t end, std::vector<const overlay*> &out) const {
        query_range_rec(root, begin, end, out);
    }

    void query_point(uint32_t pos, std::vector<const overlay*> &out) const {
        query_range(pos, pos + 1, out);
    }

    index_interval_tree<value_type> substring(uint32_t sub_begin, uint32_t sub_end) const {
        index_interval_tree<value_type> res;
        auto matches = query_range(sub_begin, sub_end);
        for (auto *ov : matches) {
            overlay adj = *ov;
            adj.begin = std::max(adj.begin, sub_begin) - sub_begin;
            adj.end = std::min(adj.end, sub_end) - sub_begin;
            res.insert(adj);
        }
        return res;
    }

private:
    std::vector<node> nodes;
    int root = -1;
    std::mt19937 rng{std::random_device{}()};

    void query_range_rec(int idx, uint32_t qbegin, uint32_t qend, std::vector<const overlay*> &out) const {
        if (idx == -1 || nodes[idx].max_end < qbegin)
            return;
        if (nodes[idx].left != -1)
            query_range_rec(nodes[idx].left, qbegin, qend, out);
        const auto &iv = nodes[idx].interval;
        if (iv.begin < qend && iv.end > qbegin)
            out.push_back(&iv);
        if (iv.begin < qend)
            query_range_rec(nodes[idx].right, qbegin, qend, out);
    }
};

