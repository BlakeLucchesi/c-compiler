//  Copyright © 2020 Blake Lucchesi. All rights reserved.

#ifndef keywords_h
#define keywords_h

#include <stdio.h>
#include <stdbool.h>
#include "Lexer.h"

static const char *KEYWORDS[] = {"", "if", "else", "return"};
static const TokenName KEYWORD_TOKEN[] = {UNDEFINED_TOKEN_NAME, KEYWORD_IF, KEYWORD_ELSE, KEYWORD_RETURN};

static const char *TYPES[] = {"int", "char"};
size_t is_keyword(char *input);
bool is_type(char *input);

#endif /* keywords_h */
