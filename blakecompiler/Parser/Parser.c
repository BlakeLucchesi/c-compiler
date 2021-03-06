//  Copyright © 2020 Blake Lucchesi. All rights reserved.

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "Parser.h"
#include "Debug.h"

void ASTReportError(Token *token, char *message);
static ASTError *_ASTErrors;

ASTExpression *parse_expression(Token **start);

bool is_unary_operator(Token *token) {
    return token->klass == OPERATOR && (token->name == OP_NEGATION || token->name == OP_LOGICAL_NEGATION || token->name == OP_BITWISE_COMPLEMENT);
}

void next_token(Token **start) {
    *start = (*start)->next;
}

Token *peek_token(Token *current) {
    return current->next;
}

ASTExpression *parse_factor(Token **start) {
    Token *current = *start;
    if (current->klass == LITERAL) {
        ASTExpression *exp = (ASTExpression *)calloc(1, sizeof(ASTExpression));
        exp->value = current->value;
        next_token(start);
        return exp;
    }
    else if (is_unary_operator(current)) {
        ASTExpression *exp = (ASTExpression *)calloc(1, sizeof(ASTExpression));
        ASTUnaryOperator *op = (ASTUnaryOperator *)calloc(1, sizeof(ASTUnaryOperator));
        op->op = current->value;
        next_token(start);
        op->expression = parse_factor(start);
        exp->unary_op = op;
        return exp;
    }
    else if (current->klass == SEPARATOR) {
        if (current->name == SEP_PAREN_OPEN) {
            next_token(start);
            ASTExpression *exp = parse_expression(start);
            current = *start;
            if (current->name == SEP_PAREN_CLOSE) {
                next_token(start);
            }
            else {
                ASTReportError(current, "Missing matching close parenthesis");
            }
            return exp;
        }
        else {
            ASTReportError(current, "Invalid separator token in term");
        }
    }
    ASTReportError(current, "Could not parse factor");
    return NULL;
}

ASTExpression *parse_term(Token **start) {
    ASTExpression *exp = parse_factor(start);
    Token *current = *start;
    while (current->klass == OPERATOR && (current->name == OP_MULTIPLICATION || current->name == OP_DIVISOR)) {
        next_token(start);
        ASTExpression *next_exp = (ASTExpression *)calloc(1, sizeof(ASTExpression));
        next_exp->binary_op = (ASTBinaryOperator *)calloc(1, sizeof(ASTBinaryOperator));
        next_exp->binary_op->op = current->value;
        next_exp->binary_op->lhs = exp;
        next_exp->binary_op->rhs = parse_factor(start);
        exp = next_exp;
        current = *start;
    }
    return exp;
}

ASTExpression *parse_expression(Token **start) {
    ASTExpression *exp = parse_term(start);
    Token *current = *start;
    while (current->klass == OPERATOR && (current->name == OP_ADDITION || current->name == OP_NEGATION)) {
        next_token(start);
        ASTExpression *next_exp = (ASTExpression *)calloc(1, sizeof(ASTExpression));
        next_exp->binary_op = (ASTBinaryOperator *)calloc(1, sizeof(ASTBinaryOperator));
        next_exp->binary_op->op = current->value;
        next_exp->binary_op->lhs = exp;
        next_exp->binary_op->rhs = parse_term(start);
        exp = next_exp;
        current = *start;
    }
    return exp;
}

ASTStatement *parse_statement(Token **start) {
    ASTStatement *statement = (ASTStatement*)malloc(sizeof(ASTStatement));
    Token *current = *start;
    if (!(current->klass == KEYWORD && current->name == KEYWORD_RETURN)) {
        ASTReportError(current, "Statement must begin with return keyword");
        return NULL;
    }
    current = current->next;
    statement->expression = parse_expression(&current);
    if (current->klass == SEPARATOR && current->name == SEP_SEMICOLON){
        current = current->next;
        *start = current;
        return statement;
    }
    ASTReportError(current, "Missing semicolon at end of statement");
    return NULL;
}

static int checkFunctionState(Token *current) {
    static int pos = 0;
    if (pos == 6)
        return 0;
    Token states[6] = {
        { .klass = KEYWORD, .name = KEYWORD_RETURN },
        { .klass = IDENTIFIER, .name = 0 },
        { .klass = SEPARATOR, .name = SEP_PAREN_OPEN },
        { .klass = SEPARATOR, .name = SEP_PAREN_CLOSE },
        { .klass = SEPARATOR, .name = SEP_BRACE_OPEN },
        { .klass = SEPARATOR, .name = SEP_BRACE_CLOSE },
    };
    if (states[pos].klass != current->klass && states[pos].name != current->name) {
        ASTReportError(current, "Invalid token in function definition");
        return 0;
    }
    pos++;
    return 1;
}

ASTFunction *parse_function(Token **start) {
    Token *current = *start;
    uint pos = 0;
    Token states[6] = {
        { .klass = KEYWORD, .name = KEYWORD_RETURN },
        { .klass = IDENTIFIER, .name = 0 },
        { .klass = SEPARATOR, .name = SEP_PAREN_OPEN },
        { .klass = SEPARATOR, .name = SEP_PAREN_CLOSE },
        { .klass = SEPARATOR, .name = SEP_BRACE_OPEN },
        { .klass = SEPARATOR, .name = SEP_BRACE_CLOSE },
    };
    ASTFunction *fn = malloc(sizeof(ASTFunction));
    fn->details.line = current->line_number;
    fn->details.start = current->col_number;
    while (current != NULL) {
        if (!checkFunctionState(current))
            return NULL;
        switch (current->klass) {
            case SEPARATOR:
                if (states[pos].name == SEP_BRACE_OPEN) {
                    current = current->next;
                    pos++;
                    fn->statement = parse_statement(&current);
                    if (fn->statement == NULL) {
                        return NULL;
                    }
                    break;
                }
                else if (states[pos].name == SEP_BRACE_CLOSE) {
                    current = current->next;
                    pos++;
                    *start = current;
                    return fn;
                }
                else {
                    current = current->next;
                    pos++;
                }
                break;
            case IDENTIFIER:
                fn->identifier = malloc(sizeof(ASTIdentifier));
                fn->identifier->name = current->value;
                fn->identifier->details.line = current->line_number;
                fn->identifier->details.start = current->col_number;
                current = current->next;
                pos++;
                break;
            case KEYWORD:
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
                ASTReportError(current, "Unexpected token");
                return NULL;
        }
    }
    ASTReportError(current, "Could not parse function");
    return NULL;
}

ASTProgram *ASTParse(Token *start) {
    ASTProgram *program = (ASTProgram*)malloc(sizeof(ASTProgram));
    Token *current = start;
    while (current != NULL) {
        if (current->klass == KEYWORD) {
            program->function = parse_function(&current);
            if (program->function == NULL) {
                return NULL;
            }
        }
        else if (current->klass == COMMENT) {
            current = current->next;
        }
        else {
            ASTReportError(current, "Unexpected token");
            return NULL;
        }
    }
    return program;
}

void ASTReportError(Token *token, char *message) {
    static ASTError *errorTail;
    ASTError *error = (ASTError*)calloc(1, sizeof(ASTError));
    error->message = message;
    error->token = token;
    if (_ASTErrors == NULL) {
        _ASTErrors = error;
        errorTail = error;
    }
    else {
        errorTail->next = error;
        errorTail = error;
    }
}

ASTError *ASTGetErrors(void) {
    return _ASTErrors;
}

void ASTPrintError(ASTError *error) {
    if (error->token == NULL) {
        debug("Failed parsing. %s. Token NULL.", error->message);
    }
    else {
        debug("Failed parsing. %s. Found token class: %d, value: %s on ln %d, col %d", error->message, error->token->klass, error->token->value, error->token->line_number, error->token->col_number);
    }
}
