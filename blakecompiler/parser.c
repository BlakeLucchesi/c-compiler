//  Copyright © 2020 Blake Lucchesi. All rights reserved.

#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "debug.h"

void fail(void);

AST_Expression *parse_expression(token **start) {
    AST_Expression *expression = (AST_Expression*)calloc(1, sizeof(AST_Expression));
    token *current = *start;
    switch (current->klass) {
        case OPERATOR: {
            switch (current->op) {
                case OP_LOGICAL_NEGATION:
                case OP_NEGATION:
                case OP_BITWISE_COMPLEMENT: {
                    AST_Unary_Operator *uop = (AST_Unary_Operator *)malloc(sizeof(AST_Unary_Operator));
                    uop->value = current->value;
                    expression->unary_operator = uop;
                    *start = current->next;
                    expression->expression = parse_expression(start);
                    return expression;
                }
                default:
                    fail();
            }
        }
        case LITERAL: {
            expression->value = current->value;
            *start = current->next;
            return expression;
        }
        default:
            fail();
    }
    fail();
    return NULL;
}

AST_Statement *parse_statement(token **start) {
    AST_Statement *statement = (AST_Statement*)malloc(sizeof(AST_Statement));
    token *current = *start;
    if (current->klass != KEYWORD && strcmp(current->value, "return") != 0) {
        fail();
    }
    current = current->next;
    statement->expression = parse_expression(&current);
    if (current->klass != SEPARATOR && current->value[0] != ';'){
        fail();
    }
    current = current->next;
    *start = current;
    return statement;
}

AST_Function *parse_function(token **start) {
    token *current = *start;
    uint pos = 0;
    TokenClass states[6] = {
        KEYWORD,
        IDENTIFIER,
        SEPARATOR,
        SEPARATOR,
        SEPARATOR,
        SEPARATOR,
    };
    AST_Function *function = malloc(sizeof(AST_Function));
    while (current != NULL) {
        switch (current->klass) {
            case UNDEFINED_TOKEN:
            case OPERATOR:
                fail();
            case SEPARATOR:
                if (states[pos] != SEPARATOR)
                    fail();
                current = current->next;
                pos++;
                if (pos == 5) {
                    function->statement = parse_statement(&current);
                }
                else if (pos == 6) {
                    *start = current;
                    return function;
                }
                break;
            case IDENTIFIER:
                if (states[pos] != IDENTIFIER)
                    fail();
                function->identifier = malloc(sizeof(AST_Identifier));
                function->identifier->name = current->value;
                current = current->next;
                pos++;
                break;
            case KEYWORD:
                if (states[pos] != KEYWORD)
                    fail();
                // TODO capture return type.
                current = current->next;
                pos++;
                break;
            case LITERAL:
                fail();
                break;
            case COMMENT:
                // TODO: add comments to AST
                current = current->next;
                break;
        }
    }
    fail();
    return NULL;
}

AST_Program *parse(token **start) {
    AST_Program *program = (AST_Program*)malloc(sizeof(AST_Program));
    token *current = *start;
    while (current != NULL) {
        if (current->klass == KEYWORD) {
            program->function = parse_function(&current);
        }
        else if (current->klass == COMMENT) {
            current = current->next;
        }
        else {
            fail();
        }
    }
    *start = current;
    return program;
}

void fail() {
    debug("Failed parsing");
    exit(EXIT_FAILURE);
}
