//  Copyright © 2020 Blake Lucchesi. All rights reserved.

#ifndef ASTDebugGenerate_h
#define ASTDebugGenerate_h
#include <stdio.h>
#include "Parser.h"
#include "Generator.h"

void ASTDebugProgram(ASTProgram *program, GeneratorConfig *config);

void ASTDebugFunction(ASTFunction *function, GeneratorConfig *config);
void ASTDebugStatement(ASTStatement *statement, GeneratorConfig *config);
void ASTDebugExpression(ASTExpression *expression, GeneratorConfig *config);

#endif /* ASTDebugGenerate_h */
