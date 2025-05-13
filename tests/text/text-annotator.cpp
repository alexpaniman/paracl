#include "paracl/text/text-annotator.h"
#include "catch2/catch2.h"
#include "paracl/text/colored-text.h"

std::vector<std::string> split(std::string input) {
    std::vector<std::string> result;
    size_t start = 0;
    size_t end = input.find(' ');

    while (end != std::string::npos) {
        result.push_back(input.substr(start, end - start));
        start = end + 1;
        end = input.find(' ', start);
    }

    if (start < input.size())
        result.push_back(input.substr(start));

    return result;
}

char get_or_default(const std::string &text, size_t i, char fallback) {
    return i < text.size() ? text[i] : fallback;
}

bool verify_straight(paracl::colored_text colored) {
    auto text = split(colored.get_text());
    for (size_t i = 0; i < text.size(); ++ i)
        for (size_t j = 0; j < text[i].size(); ++ j) {
            char symbol = text[i][j];
            if (symbol == '|' || symbol == '+')
                for (int k = i - 1; k >= 0; -- k) {
                    if (text[k][j] == '^')
                        break;

                    if (text[k][j] != '|')
                        return false;
                }

            if (symbol == '+')
                for (int k = i + 1; k >= 0; -- k) {
                    if (text[k][j] == ' ')
                        break;

                    if (text[k][j] != '-')
                        return false;
                }
        }

    return true;
}



TEST_CASE("text annotator tests") {
    using namespace paracl;

    SECTION("arrows are straight") {
        annotation_config conf {
            .line_format = {
                .foreground_color = std::nullopt,
                .background_color = std::nullopt,
                .attribute = std::nullopt,
            },
            .arrow_config = {
                .underline_connection = "^",
                .underline = "~",
                .upwards_line = "│",
                .leftwards_upwards_join = "└",
                .leftwards_line = "─",
            },
            .line_numbers_space = 5
        };

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

        std::vector<annotated_range> ranges;
        ranges.push_back({
            .range = {
                text_position{0, 1, 0},
                text_position{4, 1, 4}
            },
            .annotation = "hi",
            .color = std::nullopt
        });
        // TODO: add another ranges

        auto result = annotate(input, ranges, conf);
        result.print();
    }

}
