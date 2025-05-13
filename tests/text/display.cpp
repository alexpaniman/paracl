#include "paracl/text/display.h"
#include "catch2/catch2.h"

paracl::text_position get_position(std::span<char> text, size_t i) {
    REQUIRE(i < text.size());

    paracl::text_position pos = {0, 1, 0};
    std::span<char>::iterator iter = text.begin();

    while (iter != text.end()) {
        ++ pos.point;

        auto prev_iter = iter ++;
        if (*prev_iter == '\n') {
            pos.line ++;
            pos.column = 0;

            continue;
        }

        ++ pos.column;
    }

    pos.point = i;

    return pos;
}


TEST_CASE("display") {
    using namespace paracl;

    SECTION("text_position from point") {
        std::string input = R"(
            int main() {
                int x;
                if (x == 4) {
                    print(5);
                    x += 4;
                }

            !=

                if (x <= 8) { $
                    print(7);
                    x += 4; \\
                }
            }
        )";

        for (size_t i = 0; i < input.size(); ++ i) {
            // TODO: ~~~

            // auto pos0 = get_position(input, i);
            // auto pos1 = text_position::from_point(input, i);

            // REQUIRE(pos0.point == pos1.point);
            // REQUIRE(pos0.line == pos1.line);
            // REQUIRE(pos0.column == pos1.column);
        }

}

}
