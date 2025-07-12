#include "paracl/lexer/lexer.h"
#include "catch2/catch2.h"


#define IS_SAME(TOKEN, STRING_REPRESENTATION) \
    REQUIRE(describe_token(TOKEN) == STRING_REPRESENTATION)


TEST_CASE("tokenize basic input") {
    using namespace paracl;

    SECTION("empty input") {
        std::string input = {};
        auto tokens = tokenize(input);

        REQUIRE(tokens.empty());
    }

    SECTION("single identifier") {
        std::string input = "variable";
        auto tokens = tokenize(input);

        REQUIRE(tokens.size() == 1);
        IS_SAME(tokens[0], "ID{variable}");
    }

    SECTION("single number") {
        std::string input = "1234";
        auto tokens = tokenize(input);

        REQUIRE(tokens.size() == 1);
        IS_SAME(tokens[0], "NUMBER{1234}");
    }

    SECTION("mixed identifiers and numbers") {
        std::string input = "variable 1234";
        auto tokens = tokenize(input);

        REQUIRE(tokens.size() == 2);
        IS_SAME(tokens[0], "ID{variable}");
        IS_SAME(tokens[1], "NUMBER{1234}");
    }

    SECTION("whitespace handling") {
        std::string input = "var  1234   another_var";
        auto tokens = tokenize(input);

        REQUIRE(tokens.size() == 3);
        IS_SAME(tokens[0], "ID{var}");
        IS_SAME(tokens[1], "NUMBER{1234}");
        IS_SAME(tokens[2], "ID{another_var}");
    }

    SECTION("small paracl example") {
        std::string input = R"(
            int main() {
                int x;
                if (x == 4) {
                    print(5);
                    x += 4;
                }

                if (x <= 8) {
                    print(7);
                    x += 4;
                }
            }
        )";

        auto tokens = tokenize(input);
        REQUIRE(tokens.size() == 43);

        IS_SAME(tokens[0], "ID{int}");
        IS_SAME(tokens[1], "ID{main}");
        IS_SAME(tokens[2], "LEFT_PARENTHESIS");
        IS_SAME(tokens[3], "RIGHT_PARENTHESIS");
        IS_SAME(tokens[4], "LEFT_CURLY_BRACKET");
        IS_SAME(tokens[5], "ID{int}");
        IS_SAME(tokens[6], "ID{x}");
        IS_SAME(tokens[7], "SEMICOLON");
        IS_SAME(tokens[8], "IF");
        IS_SAME(tokens[9], "LEFT_PARENTHESIS");
        IS_SAME(tokens[10], "ID{x}");
        IS_SAME(tokens[11], "EQUAL");
        IS_SAME(tokens[12], "NUMBER{4}");
        IS_SAME(tokens[13], "RIGHT_PARENTHESIS");
        IS_SAME(tokens[14], "LEFT_CURLY_BRACKET");
        IS_SAME(tokens[15], "ID{print}");
        IS_SAME(tokens[16], "LEFT_PARENTHESIS");
        IS_SAME(tokens[17], "NUMBER{5}");
        IS_SAME(tokens[18], "RIGHT_PARENTHESIS");
        IS_SAME(tokens[19], "SEMICOLON");
        IS_SAME(tokens[20], "ID{x}");
        IS_SAME(tokens[21], "PLUS_ASSIGN");
        IS_SAME(tokens[22], "NUMBER{4}");
        IS_SAME(tokens[23], "SEMICOLON");
        IS_SAME(tokens[24], "RIGHT_CURLY_BRACKET");
        IS_SAME(tokens[25], "IF");
        IS_SAME(tokens[26], "LEFT_PARENTHESIS");
        IS_SAME(tokens[27], "ID{x}");
        IS_SAME(tokens[28], "LESS_OR_EQUAL");
        IS_SAME(tokens[29], "NUMBER{8}");
        IS_SAME(tokens[30], "RIGHT_PARENTHESIS");
        IS_SAME(tokens[31], "LEFT_CURLY_BRACKET");
        IS_SAME(tokens[32], "ID{print}");
        IS_SAME(tokens[33], "LEFT_PARENTHESIS");
        IS_SAME(tokens[34], "NUMBER{7}");
        IS_SAME(tokens[35], "RIGHT_PARENTHESIS");
        IS_SAME(tokens[36], "SEMICOLON");
        IS_SAME(tokens[37], "ID{x}");
        IS_SAME(tokens[38], "PLUS_ASSIGN");
        IS_SAME(tokens[39], "NUMBER{4}");
        IS_SAME(tokens[40], "SEMICOLON");
        IS_SAME(tokens[41], "RIGHT_CURLY_BRACKET");
        IS_SAME(tokens[42], "RIGHT_CURLY_BRACKET");
    }

    SECTION("failing paracl example") {
        std::string input = R"(
            int main() {
                int x;
                if (x == 4) {
                    print(5);
                    x += 4;
                }

                $

                if (x <= 8) {
                    print(7);
                    x += 4;
                }
            }
        )";

        REQUIRE_THROWS(tokenize(input));
    }

    SECTION("failing paracl example") {
        std::string input = R"(
            i
            if
            w
            wh
            whi
            whil
            while
        )";

        auto tokens = tokenize(input);
        REQUIRE(tokens.size() == 7);

        IS_SAME(tokens[0], "ID{i}");
        IS_SAME(tokens[1], "IF");
        IS_SAME(tokens[2], "ID{w}");
        IS_SAME(tokens[3], "ID{wh}");
        IS_SAME(tokens[4], "ID{whi}");
        IS_SAME(tokens[5], "ID{whil}");
        IS_SAME(tokens[6], "WHILE");
    }

    SECTION("failing paracl example") {
        std::string input = R"(
            if123
            ifif
            ifwhile
            whileif
            whilewhile
            ifani
            ifvani
            ifsani
            while2410
            while228
            while1337
            whileani
            whilevani
            whilesani
        )";

        auto tokens = tokenize(input);
        REQUIRE(tokens.size() == 14);

        IS_SAME(tokens[0], "ID{if123}");
        IS_SAME(tokens[1], "ID{ifif}");
        IS_SAME(tokens[2], "ID{ifwhile}");
        IS_SAME(tokens[3], "ID{whileif}");
        IS_SAME(tokens[4], "ID{whilewhile}");
        IS_SAME(tokens[5], "ID{ifani}");
        IS_SAME(tokens[6], "ID{ifvani}");
        IS_SAME(tokens[7], "ID{ifsani}");
        IS_SAME(tokens[8], "ID{while2410}");
        IS_SAME(tokens[9], "ID{while228}");
        IS_SAME(tokens[10], "ID{while1337}");
        IS_SAME(tokens[11], "ID{whileani}");
        IS_SAME(tokens[12], "ID{whilevani}");
        IS_SAME(tokens[13], "ID{whilesani}");
    }

    SECTION("identifiers can have uppercase letters") {
        std::string input = R"(
            while
            WHILE
            whilE
            whIlE
            wHIlE
            wHile
            While
            WHile
            WHIle
            WHILe
            WHILE
            WhiLe
            wHIlE
            if
            If
            IF
            iF
            IF123
            IFIF
            IFWHILE
            WHILEIF
            WHILEWHILE
            IFANI
            IFSANI
            WHILE2410
            WHILE228
            WHILE1337
            WHILEANI
            WHILESANI
        )";

        auto tokens = tokenize(input);
        REQUIRE(tokens.size() == 29);

        IS_SAME(tokens[0], "WHILE");
        IS_SAME(tokens[1], "ID{WHILE}");
        IS_SAME(tokens[2], "ID{whilE}");
        IS_SAME(tokens[3], "ID{whIlE}");
        IS_SAME(tokens[4], "ID{wHIlE}");
        IS_SAME(tokens[5], "ID{wHile}");
        IS_SAME(tokens[6], "ID{While}");
        IS_SAME(tokens[7], "ID{WHile}");
        IS_SAME(tokens[8], "ID{WHIle}");
        IS_SAME(tokens[9], "ID{WHILe}");
        IS_SAME(tokens[10], "ID{WHILE}");
        IS_SAME(tokens[11], "ID{WhiLe}");
        IS_SAME(tokens[12], "ID{wHIlE}");
        IS_SAME(tokens[13], "IF");
        IS_SAME(tokens[14], "ID{If}");
        IS_SAME(tokens[15], "ID{IF}");
        IS_SAME(tokens[16], "ID{iF}");
        IS_SAME(tokens[17], "ID{IF123}");
        IS_SAME(tokens[18], "ID{IFIF}");
        IS_SAME(tokens[19], "ID{IFWHILE}");
        IS_SAME(tokens[20], "ID{WHILEIF}");
        IS_SAME(tokens[21], "ID{WHILEWHILE}");
        IS_SAME(tokens[22], "ID{IFANI}");
        IS_SAME(tokens[23], "ID{IFSANI}");
        IS_SAME(tokens[24], "ID{WHILE2410}");
        IS_SAME(tokens[25], "ID{WHILE228}");
        IS_SAME(tokens[26], "ID{WHILE1337}");
        IS_SAME(tokens[27], "ID{WHILEANI}");
        IS_SAME(tokens[28], "ID{WHILESANI}");
    }

    SECTION("commas") {
        std::string input = R"(
            if (add(17, 23) == 8) {
                print(ani, sasha);
                int x = 0, y = 1;
            }
        )";

        auto tokens = tokenize(input);
        REQUIRE(tokens.size() == 29);

        IS_SAME(tokens[0], "IF");
        IS_SAME(tokens[1], "LEFT_PARENTHESIS");
        IS_SAME(tokens[2], "ID{add}");
        IS_SAME(tokens[3], "LEFT_PARENTHESIS");
        IS_SAME(tokens[4], "NUMBER{17}");
        IS_SAME(tokens[5], "COMMA");
        IS_SAME(tokens[6], "NUMBER{23}");
        IS_SAME(tokens[7], "RIGHT_PARENTHESIS");
        IS_SAME(tokens[8], "EQUAL");
        IS_SAME(tokens[9], "NUMBER{8}");
        IS_SAME(tokens[10], "RIGHT_PARENTHESIS");
        IS_SAME(tokens[11], "LEFT_CURLY_BRACKET");
        IS_SAME(tokens[12], "ID{print}");
        IS_SAME(tokens[13], "LEFT_PARENTHESIS");
        IS_SAME(tokens[14], "ID{ani}");
        IS_SAME(tokens[15], "COMMA");
        IS_SAME(tokens[16], "ID{sasha}");
        IS_SAME(tokens[17], "RIGHT_PARENTHESIS");
        IS_SAME(tokens[18], "SEMICOLON");
        IS_SAME(tokens[19], "ID{int}");
        IS_SAME(tokens[20], "ID{x}");
        IS_SAME(tokens[21], "ASSIGN");
        IS_SAME(tokens[22], "NUMBER{0}");
        IS_SAME(tokens[23], "COMMA");
        IS_SAME(tokens[24], "ID{y}");
        IS_SAME(tokens[25], "ASSIGN");
        IS_SAME(tokens[26], "NUMBER{1}");
        IS_SAME(tokens[27], "SEMICOLON");
        IS_SAME(tokens[28], "RIGHT_CURLY_BRACKET");
    }
}

