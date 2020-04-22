//  Copyright © 2020 Blake Lucchesi. All rights reserved.

#include "GenerateAssembly.h"
#include "Generator.h"
#include "Debug.h"

void AssembleProgram(ASTProgram *program, GeneratorConfig *config) {
    AssembleFunction(program->function, config);
}

void AssembleFunction(ASTFunction *function, GeneratorConfig *config) {
    fprintf(config->io_buffer, ".globl _%s\n", function->identifier->name);
    fprintf(config->io_buffer, "_%s:\n", function->identifier->name);
    AssembleStatement(function->statement, config);
    fprintf(config->io_buffer, " ret\n");
}

void AssembleStatement(ASTStatement *statement, GeneratorConfig *config) {
    AssembleExpression(statement->expression, config);
}

void AssembleExpression(ASTExpression *expression, GeneratorConfig *config) {
    if (expression->value) {
        fprintf(config->io_buffer, " movl $%s, %%eax\n", expression->value);
    }
    else {
        if (expression->unary_op != NULL) {
            ASTUnaryOperator *current = expression->unary_op;
            if (*(current->op) == '-') {
                AssembleExpression(current->expression, config);
                fprintf(config->io_buffer, " neg %%eax\n");
            }
            else if (*(current->op) == '!') {
                AssembleExpression(current->expression, config);
                fprintf(config->io_buffer, " cmpl $0, %%eax\n");
                fprintf(config->io_buffer, " movl $0, %%eax\n");
                fprintf(config->io_buffer, " sete %%al\n");
            }
            else if (*(current->op) == '~') {
                AssembleExpression(current->expression, config);
                fprintf(config->io_buffer, " add $1, %%eax\n");
                fprintf(config->io_buffer, " neg %%eax\n");
            }
        }
    }
}

