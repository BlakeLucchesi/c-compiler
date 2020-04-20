//  Copyright © 2020 Blake Lucchesi. All rights reserved.

#ifndef generater_h
#define generater_h

#include <stdio.h>
#include "Parser.h"
#include "Generator.h"

//void GenerateAssembly(ASTProgram *program, FILE *output);
//GeneratorConfig *CreateGeneratorAssemblyConfig(void);

void AssembleProgram(ASTProgram *program, GeneratorConfig *config);
void AssembleFunction(ASTFunction *function, GeneratorConfig *config);
void AssembleExpression(ASTExpression *expression, GeneratorConfig *config);
void AssembleStatement(ASTStatement *statement, GeneratorConfig *config);

#endif /* generater_h */
