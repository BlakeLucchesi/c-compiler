//  Copyright © 2020 Blake Lucchesi. All rights reserved.

#ifndef parser_h
#define parser_h

#include <stdio.h>
#include "Lexer.h"
#include "AST.h"

ASTProgram *ASTParse(Token *start);
ASTError *ASTGetErrors(void);
void ASTPrintError(ASTError *error);

#endif /* parser_h */
