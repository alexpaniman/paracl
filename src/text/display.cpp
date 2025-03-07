#include "paracl/text/display.h"
#include "paracl/text/colored-text.h"
#include "paracl/text/text-annotator.h"

#include <span>
#include <cassert>
#include <algorithm>


namespace paracl {

void file::message(const std::string &message, std::vector<rng> rngs) {
    assert(rngs.size() != 0);

    std::vector<annotated_range> ranges;
    for (auto rng: rngs)
        ranges.push_back(rng.actual_range);

    std::sort(ranges.begin(), ranges.end());

    std::cout << filename << ":" << ranges[0].range.begin.line << ":" << ranges[0].range.begin.column << ": " << message << "\n";

    colored_text stream = annotate(text, ranges);

    stream.print();
}

} // end namespace paracl

