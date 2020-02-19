//  Copyright © 2020 Blake Lucchesi. All rights reserved.

#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "debug.h"

void fail(void);

AST_Expression *parse_expression(token **start) {
    AST_Expression *expression = (AST_Expression*)calloc(1, sizeof(AST_Expression));
    token *current = *start;
    switch (current->name) {
        case BITWISE_COMPLEMENT:
        case NEGATION:
        case LOGICAL_NEGATION: {
            AST_Unary_Operator *uop = (AST_Unary_Operator *)malloc(sizeof(AST_Unary_Operator));
            uop->value = current->value;
            expression->unary_operator = uop;
            *start = current->next;
            expression->expression = parse_expression(start);
            return expression;
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
    if (current->name != KEYWORD && strcmp(current->value, "return") != 0) {
        fail();
    }
    current = current->next;
    statement->expression = parse_expression(&current);
    if (current->name != SEPARATOR && current->value[0] != ';'){
        fail();
    }
    current = current->next;
    *start = current;
    return statement;
}

AST_Function *parse_function(token **start) {
    token *current = *start;
    uint pos = 0;
    TokenName states[6] = {
        KEYWORD,
        IDENTIFIER,
        SEPARATOR,
        SEPARATOR,
        SEPARATOR,
        SEPARATOR,
    };
    AST_Function *function = malloc(sizeof(AST_Function));
    while (current != NULL) {
        switch (current->name) {
            case NEGATION:
            case LOGICAL_NEGATION:
            case BITWISE_COMPLEMENT:
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
        if (current->name == KEYWORD) {
            program->function = parse_function(&current);
        }
        else if (current->name == COMMENT) {
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
