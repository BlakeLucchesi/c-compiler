//  Copyright © 2020 Blake Lucchesi. All rights reserved.

#include "generater.h"
#include "debug.h"

void process_function(AST_Function *function, FILE *output);
void process_statement(AST_Statement *statement, FILE *output);
void process_expression(AST_Expression *expression, FILE *output);

void generate(AST_Program *program, FILE *output) {
    process_function(program->function, output);
}

void process_function(AST_Function *function, FILE *output) {
    fprintf(output, ".globl _%s\n", function->identifier->name);
    fprintf(output, "_%s:\n", function->identifier->name);
    process_statement(function->statement, output);
    fprintf(output, " ret\n");
}

void process_expression(AST_Expression *expression, FILE *output) {
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

void process_statement(AST_Statement *statement, FILE *output) {
    process_expression(statement->expression, output);
}
