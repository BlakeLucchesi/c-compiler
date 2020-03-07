//  Copyright © 2020 Blake Lucchesi. All rights reserved.

#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "debug.h"

void fail(Token *token, char *message);

ASTExpression *parse_expression(Token **start) {
    ASTExpression *expression = (ASTExpression*)calloc(1, sizeof(ASTExpression));
    Token *current = *start;
    switch (current->klass) {
        case OPERATOR: {
            switch (current->op) {
                case OP_LOGICAL_NEGATION:
                case OP_NEGATION:
                case OP_BITWISE_COMPLEMENT: {
                    ASTUnaryOperator *uop = (ASTUnaryOperator *)malloc(sizeof(ASTUnaryOperator));
                    uop->value = current->value;
                    expression->unary_operator = uop;
                    *start = current->next;
                    expression->expression = parse_expression(start);
                    return expression;
                }
                default:
                    fail(current, "Unsupported operator");
            }
        }
        case LITERAL: {
            expression->value = current->value;
            *start = current->next;
            return expression;
        }
        default:
            fail(current, "Expression expecting either an operator or literal");
            return NULL;
    }
}

ASTStatement *parse_statement(Token **start) {
    ASTStatement *statement = (ASTStatement*)malloc(sizeof(ASTStatement));
    Token *current = *start;
    if (current->klass != KEYWORD && strcmp(current->value, "return") != 0) {
        fail(current, "Statement must begin with return keyword");
    }
    current = current->next;
    statement->expression = parse_expression(&current);
    if (current->klass == SEPARATOR && current->value[0] == ';'){
        current = current->next;
        *start = current;
        return statement;
    }
    fail(current, "Missing semicolon at end of statement");
    return NULL;
}

ASTFunction *parse_function(Token **start) {
    Token *current = *start;
    uint pos = 0;
    Token states[6] = {
        { .klass = KEYWORD, .key = KEYWORD_RETURN },
        { .klass = IDENTIFIER },
        { .klass = SEPARATOR, .sep = SEP_PAREN_OPEN },
        { .klass = SEPARATOR, .sep = SEP_PAREN_CLOSE },
        { .klass = SEPARATOR, .sep = SEP_BRACE_OPEN },
        { .klass = SEPARATOR, .sep = SEP_BRACE_CLOSE },
    };
    ASTFunction *fn = malloc(sizeof(ASTFunction));
    fn->details.line = current->line_number;
    fn->details.start = current->col_number;
    while (current != NULL) {
        switch (current->klass) {
            case SEPARATOR:
                if (states[pos].klass != current->klass && states[pos].sep != current->sep)
                    fail(current, "Invalid token in function definition");
                current = current->next;
                pos++;
                if (states[pos].sep == SEP_BRACE_CLOSE) {
                    fn->statement = parse_statement(&current);
                }
                else if (pos == 6) {
                    *start = current;
                    return fn;
                }
                break;
            case IDENTIFIER:
                if (states[pos].klass != current->klass)
                    fail(current, "Unexpected token");
                fn->identifier = malloc(sizeof(ASTIdentifier));
                fn->identifier->name = current->value;
                fn->identifier->details.line = current->line_number;
                fn->identifier->details.start = current->col_number;
                current = current->next;
                pos++;
                break;
            case KEYWORD:
                if (states[pos].klass != current->klass)
                    fail(current, "Unexpected token");
                if (current->key != KEYWORD_RETURN)
                    fail(current, "Expected return keyword");
                // TODO capture return type.
                current = current->next;
                pos++;
                break;
            case COMMENT:
                // TODO: add comments to AST
                current = current->next;
                break;
            case UNDEFINED_TOKEN:
            case OPERATOR:
            case LITERAL:
                fail(current, "Unexpected token");
                break;
        }
    }
    fail(current, "Could not parse function");
    return NULL;
}

ASTProgram *ASTParse(Token **start) {
    ASTProgram *program = (ASTProgram*)malloc(sizeof(ASTProgram));
    Token *current = *start;
    while (current != NULL) {
        if (current->klass == KEYWORD) {
            program->function = parse_function(&current);
        }
        else if (current->klass == COMMENT) {
            current = current->next;
        }
        else {
            fail(current, "Unexpected token");
        }
    }
    *start = current;
    return program;
}

void fail(Token *token, char *message) {
    if (token == NULL) {
        debug("Failed parsing. %s. Token NULL.", message);
    }
    else {
        debug("Failed parsing. %s. Found token %s on ln %d, col %d", message, token->value, token->line_number, token->col_number);
    }
    exit(EXIT_FAILURE);
}
