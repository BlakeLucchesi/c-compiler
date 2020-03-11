//  Copyright © 2020 Blake Lucchesi. All rights reserved.

#ifndef FileLexer_h
#define FileLexer_h

#include <stdio.h>
#include <stdlib.h>

#include "Lexer.h"

Lexer *CreateFileLexer(FILE *file);

#endif /* FileLexer_h */
