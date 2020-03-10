//  Copyright © 2020 Blake Lucchesi. All rights reserved.

#include "Generater.h"
#include "Debug.h"

void process_function(ASTFunction *function, FILE *output);
void process_statement(ASTStatement *statement, FILE *output);
void process_expression(ASTExpression *expression, FILE *output);

void generate(ASTProgram *program, FILE *output) {
    process_function(program->function, output);
}

void process_function(ASTFunction *function, FILE *output) {
    fprintf(output, ".globl _%s\n", function->identifier->name);
    fprintf(output, "_%s:\n", function->identifier->name);
    process_statement(function->statement, output);
    fprintf(output, " ret\n");
}

void process_expression(ASTExpression *expression, FILE *output) {
    if (expression->value) {
        fprintf(output, " movl $%s, %%eax\n", expression->value);
    }
    else {
        if (*(expression->unary_operator->value) == '-') {
            process_expression(expression->expression, output);
            fprintf(output, " neg %%eax\n");
        }
        else if (*(expression->unary_operator->value) == '!') {
            process_expression(expression->expression, output);
            fprintf(output, " cmpl $0, %%eax\n");
            fprintf(output, " movl $0, %%eax\n");
            fprintf(output, " sete %%al\n");
        }
        else if (*(expression->unary_operator->value) == '~') {
            process_expression(expression->expression, output);
            fprintf(output, " add $1, %%eax\n");
            fprintf(output, " neg %%eax\n");
        }
    }
}

void process_statement(ASTStatement *statement, FILE *output) {
    process_expression(statement->expression, output);
}
