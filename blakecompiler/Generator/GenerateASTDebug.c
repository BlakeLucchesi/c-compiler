//  Copyright © 2020 Blake Lucchesi. All rights reserved.

#include "GenerateASTDebug.h"


#define print(indent, buffer, format, ...) \
    fprintf(buffer, "%*c> ", indent, ' '); \
    fprintf(buffer, format "\n", ##__VA_ARGS__);

void ASTDebugProgram(ASTProgram *program, GeneratorConfig *config) {
    config->indent = 1;
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
    else {
        // TODO: Switch on token names not char values.
        if (*(expression->unary_operator->value) == '-') {
            print(config->indent, config->io_buffer, "UnaryOperator -");
            config->indent++;
            ASTDebugExpression(expression->expression, config);
            config->indent--;
        }
        else if (*(expression->unary_operator->value) == '!') {
            config->indent++;
            ASTDebugExpression(expression->expression, config);
            config->indent--;
            print(config->indent, config->io_buffer, "UnaryOperator !");
        }
        else if (*(expression->unary_operator->value) == '~') {
            print(config->indent, config->io_buffer, "UnaryOperator ~");
            config->indent++;
            ASTDebugExpression(expression->expression, config);
            config->indent--;
        }
    }
}
