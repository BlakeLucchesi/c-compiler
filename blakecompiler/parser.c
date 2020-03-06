//  Copyright © 2020 Blake Lucchesi. All rights reserved.

#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "debug.h"

void fail(void);

AST_Expression *parse_expression(Token **start) {
    AST_Expression *expression = (AST_Expression*)calloc(1, sizeof(AST_Expression));
    Token *current = *start;
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

AST_Statement *parse_statement(Token **start) {
    AST_Statement *statement = (AST_Statement*)malloc(sizeof(AST_Statement));
    Token *current = *start;
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

AST_Function *parse_function(Token **start) {
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
    AST_Function *function = malloc(sizeof(AST_Function));
    while (current != NULL) {
        switch (current->klass) {
            case SEPARATOR:
                if (states[pos].klass != current->klass && states[pos].sep != current->sep)
                    fail();
                current = current->next;
                pos++;
                if (states[pos].sep == SEP_BRACE_CLOSE) {
                    function->statement = parse_statement(&current);
                }
                else if (pos == 6) {
                    *start = current;
                    return function;
                }
                break;
            case IDENTIFIER:
                if (states[pos].klass != current->klass)
                    fail();
                function->identifier = malloc(sizeof(AST_Identifier));
                function->identifier->name = current->value;
                current = current->next;
                pos++;
                break;
            case KEYWORD:
                if (states[pos].klass != current->klass)
                    fail();
                if (current->key != KEYWORD_RETURN)
                    fail();
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
                fail();
                break;
        }
    }
    fail();
    return NULL;
}

AST_Program *parse(Token **start) {
    AST_Program *program = (AST_Program*)malloc(sizeof(AST_Program));
    Token *current = *start;
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
