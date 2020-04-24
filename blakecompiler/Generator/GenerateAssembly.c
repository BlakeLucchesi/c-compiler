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
        fprintf(config->io_buffer, " mov $%s, %%rax\n", expression->value);
    }
    else if (expression->unary_op != NULL) {
        ASTUnaryOperator *current = expression->unary_op;
        if (*(current->op) == '-') {
            AssembleExpression(current->expression, config);
            fprintf(config->io_buffer, " neg %%rax\n");
        }
        else if (*(current->op) == '!') {
            AssembleExpression(current->expression, config);
            fprintf(config->io_buffer, " cmp $0, %%rax\n");
            fprintf(config->io_buffer, " mov $0, %%rax\n");
            fprintf(config->io_buffer, " sete %%al\n");
        }
        else if (*(current->op) == '~') {
            AssembleExpression(current->expression, config);
            fprintf(config->io_buffer, " add $1, %%rax\n");
            fprintf(config->io_buffer, " neg %%rax\n");
        }
    }
    else if (expression->binary_op != NULL) {
        ASTBinaryOperator *current = expression->binary_op;
        if (*(current->op) == '+') {
            AssembleExpression(current->lhs, config);
            fprintf(config->io_buffer, " push %%rax\n");
            AssembleExpression(current->rhs, config);
            fprintf(config->io_buffer, " pop %%rcx\n");
            fprintf(config->io_buffer, " add %%rcx, %%rax\n");
        }
        else if (*(current->op) == '-') {
            
        }
        else if (*(current->op) == '*') {
            AssembleExpression(current->lhs, config);
            fprintf(config->io_buffer, " push %%rax\n");
            AssembleExpression(current->rhs, config);
            fprintf(config->io_buffer, " pop %%rcx\n");
            fprintf(config->io_buffer, " imul %%rcx, %%rax\n");
        }
        else if (*(current->op) == '/') {
            
        }
    }
}

