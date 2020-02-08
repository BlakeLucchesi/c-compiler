//  Copyright © 2020 Blake Lucchesi. All rights reserved.

#ifndef keywords_h
#define keywords_h

#include <stdio.h>
#include <stdbool.h>

static const char *KEYWORDS[] = {"if", "else", "return"};
static const char *TYPES[] = {"int", "char"};
bool is_keyword(char *input);
bool is_type(char *input);

#endif /* keywords_h */
