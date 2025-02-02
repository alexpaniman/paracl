#include <string>
#include <vector>
#include <span>
#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <stdexcept>

enum class token_type {
    NONE,

    ID,

    NUMBER,

    IF,
    WHILE,

    EQUAL,
    LESS,
    BIGGER,
    LESS_OR_EQUAL,
    BIGGER_OR_EQUAL,

    LEFT_PARENTHESIS,
    RIGHT_PARENTHESIS,

    LEFT_CURLY_BRACKET,
    RIGHT_CURLY_BRACKET,

    ASSIGN,

    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,

    PLUS_ASSIGN,
    MINUS_ASSIGN,
    MULTIPLY_ASSIGN,
    DIVIDE_ASSIGN,

    SCAN,

    SEMICOLON
};


const char* token_type_to_string(token_type type) {
    switch (type) {
    case token_type::NONE:                return "NONE";

    case token_type::ID:                  return "ID";

    case token_type::NUMBER:              return "NUMBER";

    case token_type::IF:                  return "IF";
    case token_type::WHILE:               return "WHILE";

    case token_type::EQUAL:               return "EQUAL";
    case token_type::LESS:                return "LESS";
    case token_type::BIGGER:              return "BIGGER";
    case token_type::LESS_OR_EQUAL:       return "LESS_OR_EQUAL";
    case token_type::BIGGER_OR_EQUAL:     return "BIGGER_OR_EQUAL";

    case token_type::LEFT_PARENTHESIS:    return "LEFT_PARENTHESIS";
    case token_type::RIGHT_PARENTHESIS:   return "RIGHT_PARENTHESIS";

    case token_type::LEFT_CURLY_BRACKET:  return "LEFT_CURLY_BRACKET";
    case token_type::RIGHT_CURLY_BRACKET: return "RIGHT_CURLY_BRACKET";

    case token_type::ASSIGN:              return "ASSIGN";

    case token_type::PLUS:                return "PLUS";
    case token_type::MINUS:               return "MINUS";
    case token_type::MULTIPLY:            return "MULTIPLY";
    case token_type::DIVIDE:              return "DIVIDE";

    case token_type::PLUS_ASSIGN:         return "PLUS_ASSIGN";
    case token_type::MINUS_ASSIGN:        return "MINUS_ASSIGN";
    case token_type::MULTIPLY_ASSIGN:     return "MULTIPLY_ASSIGN";
    case token_type::DIVIDE_ASSIGN:       return "DIVIDE_ASSIGN";

    case token_type::SCAN:                return "SCAN";

    case token_type::SEMICOLON:           return "SEMICOLON";
    }
}



struct token {
    token_type type;
    std::span<char> text;
};


inline constexpr unsigned CHAR_COUNT = 1 << (sizeof(char) * 8);


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc99-designator"

uint8_t S30[CHAR_COUNT] = {
    ['a'...'h'] = 1, // id
    ['j'...'v'] = 1, // id
    ['x'...'z'] = 1, // id
    ['_'] = 1, // id

    ['i'] = 2, // id or if
    ['w'] = 4, // id or while 

    ['0'...'9'] = 9, // number

    ['='] = 10,

    ['<'] = 11,
    ['>'] = 12,

    ['+'] = 13,
    ['-'] = 14,
    ['*'] = 15,
    ['/'] = 16,

    [';'] = 17,

    ['?'] = 18,

    ['('] = 19,
    [')'] = 20,

    ['{'] = 21,
    ['}'] = 22,

    [' '] = 31,
    ['\t'] = 31,
    ['\n'] = 31,
};

uint8_t S31[CHAR_COUNT] = {
    [' '] = 31,
    ['\t'] = 31,
    ['\n'] = 31,
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


struct lexer_state {
    uint8_t *transition_table;

    bool is_terminal;
    token_type token = token_type::NONE;
};


lexer_state STATE_ROUTING[] = {
    [0]  = { nullptr,
                  /*is_terminal=*/false                                 },

    [1]  = { S1,  /*is_terminal=*/true, token_type::ID                  },
    [2]  = { S2,  /*is_terminal=*/false                                 },
    [3]  = { S3,  /*is_terminal=*/true, token_type::IF                  },
    [4]  = { S4,  /*is_terminal=*/false                                 },
    [5]  = { S5,  /*is_terminal=*/false                                 },
    [6]  = { S6,  /*is_terminal=*/false                                 },
    [7]  = { S7,  /*is_terminal=*/false                                 },
    [8]  = { S8,  /*is_terminal=*/true, token_type::WHILE               },
    [9]  = { S9,  /*is_terminal=*/true, token_type::NUMBER              },
    [10] = { S10, /*is_terminal=*/true, token_type::ASSIGN              },
    [11] = { S11, /*is_terminal=*/true, token_type::LESS                },
    [12] = { S12, /*is_terminal=*/true, token_type::BIGGER              },
    [13] = { S13, /*is_terminal=*/true, token_type::PLUS                },
    [14] = { S14, /*is_terminal=*/true, token_type::MINUS               },
    [15] = { S15, /*is_terminal=*/true, token_type::MULTIPLY            },
    [16] = { S16, /*is_terminal=*/true, token_type::DIVIDE              },
    [17] = { S17, /*is_terminal=*/true, token_type::SEMICOLON           },
    [18] = { S18, /*is_terminal=*/true, token_type::SCAN                },
    [19] = { S19, /*is_terminal=*/true, token_type::LEFT_PARENTHESIS    },
    [20] = { S20, /*is_terminal=*/true, token_type::RIGHT_PARENTHESIS   },
    [21] = { S21, /*is_terminal=*/true, token_type::LEFT_CURLY_BRACKET  },
    [22] = { S22, /*is_terminal=*/true, token_type::RIGHT_CURLY_BRACKET },
    [23] = { S23, /*is_terminal=*/true, token_type::PLUS_ASSIGN         },
    [24] = { S24, /*is_terminal=*/true, token_type::MINUS_ASSIGN        },
    [25] = { S25, /*is_terminal=*/true, token_type::MULTIPLY_ASSIGN     },
    [26] = { S26, /*is_terminal=*/true, token_type::DIVIDE_ASSIGN       },
    [27] = { S27, /*is_terminal=*/true, token_type::EQUAL               },
    [28] = { S28, /*is_terminal=*/true, token_type::LESS_OR_EQUAL       },
    [29] = { S29, /*is_terminal=*/true, token_type::BIGGER_OR_EQUAL     },
    [30] = { S30, /*is_terminal=*/false                                 },
    [31] = { S31, /*is_terminal=*/true, token_type::NONE                },
};

#pragma clang diagnostic pop


inline constexpr uint8_t INITIAL_STATE = 30; 
inline constexpr uint8_t PHI = 0; 


std::vector<token> lex(std::span<char> input) {
    std::vector<token> tokens;

    int line_number = 1, column_number = 0;

    // TODO: join this three
    auto last_terminal_begin = input.begin();
    auto last_terminal_end   = input.end();
    token_type last_terminal_type = token_type::NONE;

    uint8_t current_state = INITIAL_STATE;

    auto iter = input.begin();
    auto emit_token = [&]() {
        if (current_state == INITIAL_STATE) {
            std::stringstream ss;
            ss << "<stdin>:";
            ss << line_number;
            ss << ":";
            ss << column_number;
            ss << ": ";
            ss << "error: lexical analysis failed, unexpected symbol: "
               << static_cast<int>(*iter) << " '" << *iter << "'\n";

            // TODO: print line with ^~~~ marker
            throw std::runtime_error(ss.str());
        }

        if (last_terminal_type != token_type::NONE) {
            std::span<char> token_text{last_terminal_begin, last_terminal_end};
            tokens.emplace_back(last_terminal_type, token_text);
        }

        iter = last_terminal_begin = last_terminal_end;
        current_state = INITIAL_STATE;
    };

    for (auto end = input.end(); iter != end; ) {
        uint8_t *state = STATE_ROUTING[current_state].transition_table;

        char symbol = *iter;

        uint8_t next_state_id = state[static_cast<int>(symbol)];

        lexer_state next_state = STATE_ROUTING[next_state_id];
        uint8_t *transition_table = next_state.transition_table;

        if (transition_table) {
            ++ iter;

            column_number ++;
            if (*iter == '\n') {
                line_number ++;
                column_number = 0;
            }
        } else {
            emit_token();
            continue;
        }

        current_state = next_state_id;

        if (next_state.is_terminal) {
            last_terminal_end = iter;
            last_terminal_type = next_state.token;
        }
    }

    emit_token();

    return tokens;
}

void print_token(const std::span<token> tokens) {
    for (token tok: tokens) {
        // TODO: don't do this, slow
        std::string text{tok.text.begin(), tok.text.end()};
        std::cout << token_type_to_string(tok.type) << "{" << text << "}\n";
    }
}

void repeated(std::stringstream &ss, const std::string &string, int count) {
    for (int i = 0; i < count; ++ i)
        ss << string;
}

void align_right(std::stringstream &ss,  const std::string &string, int desired_length) {
    int padding = desired_length - string.size();
    repeated(ss, " ", padding);
    ss << string;
}

void align_left(std::stringstream &ss,  const std::string &string, int desired_length) {
    int padding = desired_length - string.size();
    ss << string;
    repeated(ss, " ", padding);
}

void print_error() {
    std::ifstream ifs { file };

    int first_line_number = line - props.num_pred_lines;
    auto lines = read_selected_lines(ifs, first_line_number, line + props.num_succ_lines);

    int max_line_length = std::numeric_limits<int>::min();
    for (auto &&element: lines)
        max_line_length = std::max<int>(max_line_length, element.size());

    std::stringstream ss;
    ss << "┌" << repeated("─", props.line_num_space + 2) << "┬" << repeated("─", max_line_length + 2) << "┐\n";

    for (int i = 0; i < (int) lines.size(); ++ i) {
        fmt::text_style printing_style;

        bool is_target_line = i == props.num_pred_lines;
        if (is_target_line)
            printing_style = fmt::emphasis::bold | fg(fmt::color::indian_red);

        // print current line number:
        int current_line = i + first_line_number;
        // ss << "│ " << fmt::format(printing_style, "{:>{}}", current_line, props.line_num_space) << " │ ";
        ss << "│ " << fmt::format(printing_style, "{:>{}}", current_line, props.line_num_space) << " │ ";

        // print current source string:
        boxed_text += fmt::format(printing_style, "{:<{}}", lines[i], max_line_length) + " │\n";
        if (is_target_line) {
            std::string pointer = fmt::format("{:>{}}", "^", column);

            fmt::text_style pointer_style = fmt::fg(fmt::color::sky_blue) | fmt::emphasis::bold;

            boxed_text += "│ " + fmt::format(pointer_style, "{:>{}}", "!!", props.line_num_space) + " │ ";
            boxed_text += fmt::format(pointer_style, "{:<{}}", pointer, max_line_length) + " │\n";
        }
    }

    boxed_text += "└" + repeat("─", props.line_num_space + 2) + "┴" + repeat("─", max_line_length + 2) + "┘\n";
    return boxed_text;
}


int main(int argc, const char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " [FILE]\n";
        return EXIT_FAILURE;
    }

    const char* file_name = argv[1];
    std::ifstream input_file_stream{file_name};

    std::stringstream text_stream;
    text_stream << input_file_stream.rdbuf();

    std::string text = text_stream.str();
    std::vector<token> tokens = lex(text);
    print_token(tokens);
}

