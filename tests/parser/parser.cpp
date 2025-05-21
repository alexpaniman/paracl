#include "paracl/lexer/lexer.h"
#include "paracl/ast/ast.h"
#include "catch2/catch2.h"


TEST_CASE("parse tokens") {
    using namespace paracl;

    SECTION("no tokens") {
        std::vector<token> tokens = {};

        paracl::ast ast(tokens);
        std::ostringstream oss;
        ast.dump(oss);

        REQUIRE(oss.str() == "main( )");
    }

    SECTION("assign tokens") {
        std::string input = R"(
            i1 = 1;
            i2 = i1;

            i1 += 2;
            ii += i2;

            i1 -= 2;
            i1 -= i2;

            i1 *= 2;
            i1 *= i2;

            i1 /= 2;
            i1 /= i2;
        )";
        auto tokens = tokenize(input);

        paracl::ast ast(tokens);
        std::ostringstream oss;
        ast.dump(oss);

        REQUIRE(oss.str() == "main( = (i1 1) = (i2 i1) += (i1 2) += (ii i2) -= (i1 2) -= (i1 i2) *= (i1 2) *= (i1 i2) /= (i1 2) /= (i1 i2) )");
    }

    SECTION("arithmetic tokens") {
        std::string input = R"(
            i1 = 1 + 1;
            i2 = i1 + 1;
            i3 = 1 + i1;
            i4 = i1 + i1;

            i1 = 2 - 1;
            i2 = i1 - 1;
            i3 = 1 - i1;
            i4 = i1 - i1;

            i1 = 1 * 1;
            i2 = i1 * 1;
            i3 = 1 * i1;
            i4 = i1 * i1;

            i1 = 1 / 1;
            i2 = i1 / 1;
            i3 = 1 / i1;
            i4 = i1 / i1;

            i1 = -1;
            i2 = -i1;
        )";
        auto tokens = tokenize(input);

        paracl::ast ast(tokens);
        std::ostringstream oss;
        ast.dump(oss);

        REQUIRE(oss.str() == "main( = (i1 + (1 1)) = (i2 + (i1 1)) = (i3 + (1 i1)) = (i4 + (i1 i1)) = (i1 - (2 1)) = (i2 - (i1 1)) = (i3 - (1 i1)) = (i4 - (i1 i1)) = (i1 * (1 1)) = (i2 * (i1 1)) = (i3 * (1 i1)) = (i4 * (i1 i1)) = (i1 / (1 1)) = (i2 / (i1 1)) = (i3 / (1 i1)) = (i4 / (i1 i1)) = (i1 - (1)) = (i2 - (i1)) )");
    }

    SECTION("binary comparation tokens") {
        std::string input = R"(
            i1 = 1;

            i2 = 1 == i1;
            i3 = i1 == 1;
            i4 = i1 == i1;

            i2 = 1 < i1;
            i3 = i1 < 1;
            i4 = i1 < i1;

            i2 = 1 > i1;
            i3 = i1 > 1;
            i4 = i1 > i1;

            i2 = 1 <= i1;
            i3 = i1 <= 1;
            i4 = i1 <= i1;

            i2 = 1 >= i1;
            i3 = i1 >= 1;
            i4 = i1 >= i1;
        )";
        auto tokens = tokenize(input);

        paracl::ast ast(tokens);
        std::ostringstream oss;
        ast.dump(oss);

        REQUIRE(oss.str() == "main( = (i1 1) = (i2 == (1 i1)) = (i3 == (i1 1)) = (i4 == (i1 i1)) = (i2 &lt; (1 i1)) = (i3 &lt; (i1 1)) = (i4 &lt; (i1 i1)) = (i2 &gt; (1 i1)) = (i3 &gt; (i1 1)) = (i4 &gt; (i1 i1)) = (i2 &le; (1 i1)) = (i3 &le; (i1 1)) = (i4 &le; (i1 i1)) = (i2 &ge; (1 i1)) = (i3 &ge; (i1 1)) = (i4 &ge; (i1 i1)) )");
    }

    SECTION("conditional tokens") {
        std::string input = R"(
            i1 = 1;

            if(i1 == 1) {
                if(1 + 1) {
                    while(0) {
                        i2 = 0;
                    }
                }
            }
        )";
        auto tokens = tokenize(input);

        paracl::ast ast(tokens);
        std::ostringstream oss;
        ast.dump(oss);

        REQUIRE(oss.str() == "main( = (i1 1) if ((== (i1 1)) (if ((+ (1 1)) (while ((0) (= (i2 0))))))) )");
    }

    SECTION("scan token") {
        std::string input = R"(
            i1 = ?;
            i2 = 1 + ?;

            if(?) {
                i3 = ? - 2;
            }
        )";
        auto tokens = tokenize(input);

        paracl::ast ast(tokens);
        std::ostringstream oss;
        ast.dump(oss);

        REQUIRE(oss.str() == "main( = (i1 scan) = (i2 + (1 scan)) if ((scan) (= (i3 - (scan 2)))) )");
    }

    SECTION("print token") {
        std::string input = R"(
            val = 2;
            print(1);
            print(val);
            print(?);
            print(val + 1 + ?);
        )";
        auto tokens = tokenize(input);

        paracl::ast ast(tokens);
        std::ostringstream oss;
        ast.dump(oss);

        REQUIRE(oss.str() == "main( = (val 2) print( 1 ) print( val ) print( scan ) print( + (+ (val 1) scan) ) )");
    }
}