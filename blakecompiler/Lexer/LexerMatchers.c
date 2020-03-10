//  Copyright © 2020 Blake Lucchesi. All rights reserved.

#include "LexerMatchers.h"

bool is_number(char c) {
    return (int)c >= 48 && (int)c <= 57;
}

bool is_identifier_character(char c) {
    return is_letter(c) || is_number(c) || c == '_';
}

bool is_letter(char c) {
    return (int)c >= (int)'A' && (int)c <= (int)'z';
}

bool is_uppercase(char c) {
    return (int)c >= (int)'A' && (int)c <= (int)'Z';
}

bool is_lowercase(char c) {
    return (int)c >= (int)'a' && (int)c <= (int)'z';
}

bool is_whitespace(char c) {
    return c == '\n' || c == ' ' || c ==  '\r' || c == '\t';
}
