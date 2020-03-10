//  Copyright © 2020 Blake Lucchesi. All rights reserved.

#ifndef LexerMatchers_h
#define LexerMatchers_h

#include <stdio.h>
#include <stdbool.h>

bool is_number(char c);
bool is_letter(char c);
bool is_lowercase(char c);
bool is_uppercase(char c);
bool is_whitespace(char c);
bool is_identifier_character(char c);

#endif /* LexerMatchers_h */
