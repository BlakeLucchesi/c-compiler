//  Copyright © 2020 Blake Lucchesi. All rights reserved.

#include "generater.h"

void generate(AST_Program *program, FILE *output) {
    char val[] = "assembly...";
    fwrite(val, 1, sizeof(val), output);
}
