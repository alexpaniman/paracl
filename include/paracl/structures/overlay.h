#include <algorithm>
#include <concepts>
#include <vector>
#include <cstdint>
#include <span>
#include <optional>
#include <cassert>

#include "index-span.h"


namespace paracl {

struct data {
    int32_t value;
};


/// Stores non-owning text span along with overlays to that text with arbitrary data.
/// Overlays can never intersect.
struct text_overlays {
public:
    struct overlay {
        index_span<uint32_t> span;
        data value;
    };

    text_overlays(std::vector<overlay> overlays):
        overlays_(std::move(overlays)) {

        // TODO: hide this under assert somehow
        verify();
    }

    /// Create new paracl::text_overlays, preserving just overlays inside the provided span.
    /// Time complexity: O(M + log N), where N is number of overlays in current text_overlays
    /// and M is number of overlays in resulting text_overlays.
    text_overlays subspan(index_span<uint32_t> span) {
        auto lower = get_lower_bound_overlay(span.begin);
        auto upper = get_lower_bound_overlay(span.end);

        std::vector<overlay> range_overlays;
        range_overlays.resize(upper - lower);

        for (auto it = lower; it != upper; ++it) {
            index_span<uint32_t> adjusted{it->span.begin - span.begin, it->span.end - span.begin};
            range_overlays.emplace_back(adjusted, it->value);
        }
        
        return text_overlays{std::move(range_overlays)};
    }

    /// Time complexity: O(log N)
    std::optional<overlay> get_overlay_at_point(uint32_t point) {
        auto it = get_lower_bound_overlay(point);

        if (it == overlays_.end() || !it->span.contains(point))
            return std::nullopt;

        return *it;
    }

    /// Time complexity: O((end - begin) + N)
    template <std::bidirectional_iterator iterator>
    requires std::convertible_to<std::iter_value_t<iterator>, const overlay&>
    void inject_overlays(iterator begin, iterator end) {
        // begin..end overlays should be a correct text_overlay on their own
        // TODO: hide this under assert somehow
        (void) text_overlays{std::vector(begin, end)};

        if (begin == end)
            return;

        const overlay &first = *begin, &last = *(end - 1);

        auto compare_lower = [](const overlay &lhs, uint32_t rhs) { return lhs.span.end <= rhs; };
        auto lower = std::lower_bound(overlays_.begin(), overlays_.end(), first.span.begin, compare_lower);

        auto compare_upper = [](const overlay &lhs, uint32_t rhs) { return lhs.span.begin >= rhs; };
        auto upper = std::lower_bound(overlays_.rbegin(), overlays_.rend(), last.span.end, compare_upper);

        if (lower != overlays_.end() && lower->span.begin < first.span.begin) {
            lower->span.end = first.span.begin;
            ++ lower;
        }

        if (upper != overlays_.rend() && upper->span.end > last.span.end) {
            upper->span.begin = last.span.end;
            ++ upper;
        }

        if (upper != overlays_.rend()) {
            auto converted_upper = std::prev(upper.base());

            if (lower <= converted_upper)
                overlays_.erase(lower, converted_upper + 1);
        }

        overlays_.insert(lower, begin, end);

        //                    lower -v                                  v- upper
        // overlays_  = ------------ ------------- ------- ------ ------- ----
        // begin, end =                 first> ------ -- --- ------- <last

        //                                  lower -v          v- upper
        // overlays_  = ------------ ------------- ------ --- -- -------- ----
        // begin, end =                    first> ----- -- --- -- <last

        //                                      lower -v    v- upper
        // overlays_  = ------------ -----             --- --             ----
        // begin, end =                 first> ------ -- --- ------- <last

        //                               v- upper                  lower -v
        // overlays_  = ------------ -----                                ----
        // begin, end =                 first> ------ -- --- ------- <last

        //                               v- upper
        // overlays_  = ------------ ----- 
        // begin, end =                 first> ------ -- --- ------- <last

        //                                                         lower -v
        // overlays_  =                                                   ----
        // begin, end =                 first> ------ -- --- ------- <last
    }

private:
    std::vector<overlay> overlays_;
    std::span<char> text_;

    std::vector<overlay>::iterator get_lower_bound_overlay(uint32_t point) {
        auto lower_compare = [](const overlay& lhs, uint32_t rhs) { return lhs.span.begin < rhs; };
        auto lower = std::lower_bound(overlays_.begin(), overlays_.end(), point, lower_compare);

        return lower;
    }

    void verify() {
        for (const auto &overlay: overlays_) {
            // spans are never empty
            assert(overlay.span.begin != overlay.span.end);

            // unnecessary for unsigned types, but I will keep it in case template uses signed
            assert(overlay.span.begin >= 0);

            // overlays don't exceed underlying text
            assert(overlay.span.end <= text_.size());
        }

        auto prev = overlays_.begin();
        for (auto end = overlays_.end(), i = ++ overlays_.begin(); i != end; prev = i, ++ i) {
            // beginnings are sorted
            assert(prev->span.begin < i->span.begin);

            // endings are sorted
            assert(prev->span.end < i->span.end);

            // overlays do not intersect
            assert(prev->span.end <= i->span.begin);
        }
    }


};


} // end namespace paracl

