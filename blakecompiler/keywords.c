//  Copyright © 2020 Blake Lucchesi. All rights reserved.
#include <string.h>

#include "keywords.h"

bool is_keyword(char *input) {
    int keyword_count = 3;
    while (keyword_count > 0) {
        if (strcmp(input, KEYWORDS[keyword_count - 1]) == 0) {
            return true;
        }
        keyword_count--;
    }
    return false;
}

bool is_type(char *input) {
    int type_count = 2;
    while (type_count > 0) {
        if (strcmp(input, TYPES[type_count - 1]) == 0) {
            return true;
        }
        type_count--;
    }
    return false;
}
