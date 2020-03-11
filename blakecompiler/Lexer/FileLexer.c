//  Copyright © 2020 Blake Lucchesi. All rights reserved.

#include "FileLexer.h"

char file_take_next(void *data);
char file_peek_next(void *data);

Lexer *CreateFileLexer(FILE *file) {
    Lexer *lexer = CreateLexer();
    lexer->take_next = file_take_next;
    lexer->peek_next = file_peek_next;
    lexer->data = file;
    return lexer;
}

char file_take_next(void *data) {
    char c = fgetc((FILE *)data);
    return c;
}
    
char file_peek_next(void *data) {
    char next = fgetc((FILE *)data);
    fseek((FILE *)data, -1, SEEK_CUR);
    return next;
}
