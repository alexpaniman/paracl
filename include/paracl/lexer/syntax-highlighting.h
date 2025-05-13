#pragma once

#include "paracl/lexer/lexer.h"
#include "paracl/text/colored-text.h"
#include <map>

std::map<std::string, paracl::colored_text::color> tokens = {
    { "ID{main}", paracl::colored_text::color::BRIGHT_MAGENTA },
    { "ID{print}", paracl::colored_text::color::BRIGHT_MAGENTA },

// (defun simpc-types ()
//   '("char" "int" "long" "short" "void" "bool" "float" "double" "signed" "unsigned"
//     "char16_t" "char32_t" "char8_t"
//     "int8_t" "uint8_t" "int16_t" "uint16_t" "int32_t" "uint32_t" "int64_t" "uint64_t"
//     "uintptr_t"
//     "size_t"))

// (defun simpc-keywords ()
//   '("auto" "break" "case" "const" "continue" "default" "do"
//     "else" "enum" "extern" "for" "goto" "if" "register"
//     "return"  "sizeof" "static" "struct" "switch" "typedef"
//     "union"  "volatile" "while" "alignas" "alignof" "and"
//     "and_eq" "asm" "atomic_cancel" "atomic_commit" "atomic_noexcept" "bitand"
//     "bitor" "catch"  "class" "co_await"
//     "co_return" "co_yield" "compl" "concept" "const_cast" "consteval" "constexpr"
//     "constinit" "decltype" "delete" "dynamic_cast" "explicit" "export" "false" 
//     "friend" "inline" "mutable" "namespace" "new" "noexcept" "not" "not_eq"
//     "nullptr" "operator" "or" "or_eq" "private" "protected" "public" "reflexpr"
//     "reinterpret_cast" "requires" "static_assert" "static_cast" "synchronized"
//     "template" "this" "thread_local" "throw" "true" "try" "typeid" "typename"
//     "using" "virtual" "wchar_t" "xor" "xor_eq"))

};

