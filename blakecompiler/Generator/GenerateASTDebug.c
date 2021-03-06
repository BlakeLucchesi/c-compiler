//  Copyright © 2020 Blake Lucchesi. All rights reserved.

#include "GenerateASTDebug.h"


#define print(indent, buffer, format, ...) \
    fprintf(buffer, "%*c> ", indent, ' '); \
    fprintf(buffer, format "\n", ##__VA_ARGS__);

void ASTDebugProgram(ASTProgram *program, GeneratorConfig *config) {
    config->indent = 1;
    fprintf(config->io_buffer, "\n"); // start w/ new line to clear any previous output.
    print(config->indent, config->io_buffer, "Program");
    config->indent++;
    ASTDebugFunction(program->function, config);
    config->indent--;
}

void ASTDebugFunction(ASTFunction *function, GeneratorConfig *config) {
    print(config->indent, config->io_buffer, "Function _%s", function->identifier->name)
    config->indent++;
    ASTDebugStatement(function->statement, config);
    config->indent--;
}

void ASTDebugStatement(ASTStatement *statement, GeneratorConfig *config) {
    print(config->indent, config->io_buffer, "Expression");
    config->indent++;
    ASTDebugExpression(statement->expression, config);
    config->indent--;
}

void ASTDebugExpression(ASTExpression *expression, GeneratorConfig *config) {
    if (expression->value) {
        print(config->indent, config->io_buffer, "Literal %s", expression->value);
    }
    else if (expression->unary_op != NULL) {
        ASTUnaryOperator *current = expression->unary_op;
        if (*(current->op) == '-') {
            print(config->indent, config->io_buffer, "UnaryOperator -");
            config->indent++;
            ASTDebugExpression(current->expression, config);
            config->indent--;
        }
        else if (*(current->op) == '!') {
            print(config->indent, config->io_buffer, "UnaryOperator !");
            config->indent++;
            ASTDebugExpression(current->expression, config);
            config->indent--;
        }
        else if (*(current->op) == '~') {
            print(config->indent, config->io_buffer, "UnaryOperator ~");
            config->indent++;
            ASTDebugExpression(current->expression, config);
            config->indent--;
        }
    }
    else if (expression->binary_op != NULL) {
        ASTBinaryOperator *current = expression->binary_op;
        print(config->indent, config->io_buffer, "%s ", current->op);
        config->indent++;
        ASTDebugExpression(current->lhs, config);
        ASTDebugExpression(current->rhs, config);
        config->indent--;
    }
}
