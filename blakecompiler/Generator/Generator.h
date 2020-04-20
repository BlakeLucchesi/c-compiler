//  Copyright © 2020 Blake Lucchesi. All rights reserved.

#ifndef Generator_h
#define Generator_h

#include <stdio.h>
#include "Parser.h"

//typedef void (*process_program_t)(ASTProgram *program, GeneratorConfig *config);
//typedef void (*process_function_t)(ASTFunction *function, GeneratorConfig *config);
//typedef void (*process_statement_t)(ASTStatement *statement, GeneratorConfig *config);
//typedef void (*process_expression_t)(ASTExpression *expression, GeneratorConfig *config);

typedef struct _generator_handlers {
    void *io_buffer;
    int indent;
} GeneratorConfig;

//void Generate(ASTProgram *program, GeneratorConfig *config);

#endif /* Generator_h */
