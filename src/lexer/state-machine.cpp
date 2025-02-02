#include "state-machine.h"
#include "paracl/lexer/token.h"


namespace paracl {

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc99-designator"


inline constexpr size_t CHAR_COUNT = 1 << (sizeof(char) * 8);

uint8_t S30[CHAR_COUNT] = {
    ['a'...'h'] = 1, // id
    ['j'...'v'] = 1, // id
    ['x'...'z'] = 1, // id
    ['_'] = 1, // id

    ['i'] = 2, // id or if
    ['w'] = 4, // id or while

    ['0'...'9'] = 9, // number

    ['='] = 10, // assign or equal

    ['<'] = 11, // bigger or "bigger or equal"
    ['>'] = 12, // less or "less or equal"

    ['+'] = 13, // plus or plus assign
    ['-'] = 14, // minus or minus assign
    ['*'] = 15, // multiply or multiply assign
    ['/'] = 16, // divide or divide assign

    [';'] = 17, // semicolon

    ['?'] = 18, // scan

    ['('] = 19, // left parenthesis
    [')'] = 20, // right parenthesis

    ['{'] = 21, // left curly bracket
    ['}'] = 22, // right curly bracket

    [' '] = 31, // space (skipped)
    ['\t'] = 31, // tab (skipped)
    ['\n'] = 31, // line feed (skipped)
};

uint8_t S31[CHAR_COUNT] = {
    // terminal space (skipped)

    [' '] = 31, // space (skipped)
    ['\t'] = 31, // tab (skipped)
    ['\n'] = 31, // line feed (skipped)
};

uint8_t S1[CHAR_COUNT] = {
    // terminal id

    ['a'...'z'] = 1, // id
    ['0'...'9'] = 1, // id
    ['_'] = 1, // id
};

uint8_t S2[CHAR_COUNT] = {
    ['a'...'e'] = 1, // id
    ['g'...'z'] = 1, // id
    ['0'...'9'] = 1, // id
    ['_'] = 1, // id

    ['f'] = 3, // if
};

uint8_t S3[CHAR_COUNT] = {
    // terminal if
};

uint8_t S4[CHAR_COUNT] = {
    ['a'...'g'] = 1, // id
    ['i'...'z'] = 1, // id
    ['0'...'9'] = 1, // id
    ['_'] = 1, // id

    ['h'] = 5, // while
};

uint8_t S5[CHAR_COUNT] = {
    ['a'...'h'] = 1, // id
    ['j'...'z'] = 1, // id
    ['0'...'9'] = 1, // id
    ['_'] = 1, // id

    ['i'] = 6, // while
};

uint8_t S6[CHAR_COUNT] = {
    ['a'...'k'] = 1, // id
    ['m'...'z'] = 1, // id
    ['0'...'9'] = 1, // id
    ['_'] = 1, // id

    ['l'] = 7, // while
};

uint8_t S7[CHAR_COUNT] = {
    ['a'...'d'] = 1, // id
    ['f'...'z'] = 1, // id
    ['0'...'9'] = 1, // id
    ['_'] = 1, // id

    ['e'] = 8, // while
};

uint8_t S8[CHAR_COUNT] = {
    // terminal while
};

uint8_t S9[CHAR_COUNT] = {
    // terminal number
    ['0'...'9'] = 9,
};

uint8_t S10[CHAR_COUNT] = {
    ['='] = 27,
    // terminal assign
};

uint8_t S11[CHAR_COUNT] = {
    ['='] = 28,
    // terminal less
};

uint8_t S12[CHAR_COUNT] = {
    ['='] = 29,
    // terminal bigger
};

uint8_t S13[CHAR_COUNT] = {
    // terminal plus
    ['='] = 23,
};

uint8_t S14[CHAR_COUNT] = {
    // terminal minus
    ['='] = 24,
};

uint8_t S15[CHAR_COUNT] = {
    // terminal multiply
    ['='] = 25,
};

uint8_t S16[CHAR_COUNT] = {
    // terminal divide
    ['='] = 26,
};

uint8_t S17[CHAR_COUNT] = {
    // terminal semicolon
};

uint8_t S18[CHAR_COUNT] = {
    // terminal scan
};

uint8_t S19[CHAR_COUNT] = {
    // terminal left parenthesis
};

uint8_t S20[CHAR_COUNT] = {
    // terminal right parenthesis
};

uint8_t S21[CHAR_COUNT] = {
    // terminal left curly bracket
};

uint8_t S22[CHAR_COUNT] = {
    // terminal right curly bracket
};

uint8_t S23[CHAR_COUNT] = {
    // terminal plus assign
};

uint8_t S24[CHAR_COUNT] = {
    // terminal minus assign
};

uint8_t S25[CHAR_COUNT] = {
    // terminal mul assign
};

uint8_t S26[CHAR_COUNT] = {
    // terminal divide assign
};

uint8_t S27[CHAR_COUNT] = {
    // terminal equal
};

uint8_t S28[CHAR_COUNT] = {
    // terminal less or equal
};

uint8_t S29[CHAR_COUNT] = {
    // terminal bigger or equal
};

generic_lexer::state lexer_states[] = {
    [0]  = { nullptr,  /*is_terminal=*/false,                                 },
    [1]  = { S1,       /*is_terminal=*/ true, token_type::ID                  },
    [2]  = { S2,       /*is_terminal=*/false,                                 },
    [3]  = { S3,       /*is_terminal=*/ true, token_type::IF                  },
    [4]  = { S4,       /*is_terminal=*/false,                                 },
    [5]  = { S5,       /*is_terminal=*/false,                                 },
    [6]  = { S6,       /*is_terminal=*/false,                                 },
    [7]  = { S7,       /*is_terminal=*/false,                                 },
    [8]  = { S8,       /*is_terminal=*/ true, token_type::WHILE               },
    [9]  = { S9,       /*is_terminal=*/ true, token_type::NUMBER              },
    [10] = { S10,      /*is_terminal=*/ true, token_type::ASSIGN              },
    [11] = { S11,      /*is_terminal=*/ true, token_type::LESS                },
    [12] = { S12,      /*is_terminal=*/ true, token_type::BIGGER              },
    [13] = { S13,      /*is_terminal=*/ true, token_type::PLUS                },
    [14] = { S14,      /*is_terminal=*/ true, token_type::MINUS               },
    [15] = { S15,      /*is_terminal=*/ true, token_type::MULTIPLY            },
    [16] = { S16,      /*is_terminal=*/ true, token_type::DIVIDE              },
    [17] = { S17,      /*is_terminal=*/ true, token_type::SEMICOLON           },
    [18] = { S18,      /*is_terminal=*/ true, token_type::SCAN                },
    [19] = { S19,      /*is_terminal=*/ true, token_type::LEFT_PARENTHESIS    },
    [20] = { S20,      /*is_terminal=*/ true, token_type::RIGHT_PARENTHESIS   },
    [21] = { S21,      /*is_terminal=*/ true, token_type::LEFT_CURLY_BRACKET  },
    [22] = { S22,      /*is_terminal=*/ true, token_type::RIGHT_CURLY_BRACKET },
    [23] = { S23,      /*is_terminal=*/ true, token_type::PLUS_ASSIGN         },
    [24] = { S24,      /*is_terminal=*/ true, token_type::MINUS_ASSIGN        },
    [25] = { S25,      /*is_terminal=*/ true, token_type::MULTIPLY_ASSIGN     },
    [26] = { S26,      /*is_terminal=*/ true, token_type::DIVIDE_ASSIGN       },
    [27] = { S27,      /*is_terminal=*/ true, token_type::EQUAL               },
    [28] = { S28,      /*is_terminal=*/ true, token_type::LESS_OR_EQUAL       },
    [29] = { S29,      /*is_terminal=*/ true, token_type::BIGGER_OR_EQUAL     },
    [30] = { S30,      /*is_terminal=*/false,                                 },
    [31] = { S31,      /*is_terminal=*/ true, token_type::NONE                },
};

#pragma clang diagnostic pop

} // end namespace paracl
