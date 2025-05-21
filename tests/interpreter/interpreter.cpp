#include "paracl/lexer/lexer.h"
#include "paracl/ast/ast.h"
#include "catch2/catch2.h"

TEST_CASE("run ParaCL program") {
    using namespace paracl;

    SECTION("no program") {
        std::vector<token> tokens = {};

        paracl::ast ast(tokens);
        std::stringstream output;
        std::streambuf* old_cout = std::cout.rdbuf(output.rdbuf());

        ast.run();

        std::cout.rdbuf(old_cout);
        REQUIRE(output.str() == "");
    }

    SECTION("factorial") {
        std::string input = R"(
            max_border = 5;
            res        = 1;
            cur_it     = 1;

            while (cur_it <= max_border) {
                res    *= cur_it;
                cur_it += 1;
                print(res);
            }
        )";
        auto tokens = tokenize(input);

        paracl::ast ast(tokens);

        std::stringstream output;
        std::streambuf* old_cout = std::cout.rdbuf(output.rdbuf());

        ast.run();

        std::cout.rdbuf(old_cout);
        REQUIRE(output.str() == "1\n2\n6\n24\n120\n");
    }

    SECTION("fibonacci") {
        std::string input = R"(
            max_border = 8;
            cur_it     = 1;

            fn   = 0;
            fn_2 = 0;
            fn_1 = 1;

            while(cur_it < max_border) {
                fn = fn_1 + fn_2;
                fn_2 = fn_1;
                fn_1 = fn;
                cur_it += 1;
            }

            print(fn);
        )";
        auto tokens = tokenize(input);

        paracl::ast ast(tokens);

        std::stringstream output;
        std::streambuf* old_cout = std::cout.rdbuf(output.rdbuf());

        ast.run();

        std::cout.rdbuf(old_cout);
        REQUIRE(output.str() == "21\n");
    }

    SECTION("number of even digits") {
        std::string input = R"(
            num = 1234567890;
            res = 0;

            while(num > 0) {
                if(num / 2 * 2 == num) {
                    res += 1;
                }
                num /= 10;
            }

            print(res);
        )";
        auto tokens = tokenize(input);

        paracl::ast ast(tokens);

        std::stringstream output;
        std::streambuf* old_cout = std::cout.rdbuf(output.rdbuf());

        ast.run();

        std::cout.rdbuf(old_cout);
        REQUIRE(output.str() == "5\n");
    }

    SECTION("complex expression") {
        std::string input = R"(
            anishka = (3 + 5 * (4 - 8) / -4) * (6 - (2 + 3) * 2) + 10 / (5 - 3);

            print(anishka);
        )";
        auto tokens = tokenize(input);

        paracl::ast ast(tokens);

        std::stringstream output;
        std::streambuf* old_cout = std::cout.rdbuf(output.rdbuf());

        ast.run();

        std::cout.rdbuf(old_cout);
        REQUIRE(output.str() == "-27\n");
    }
}