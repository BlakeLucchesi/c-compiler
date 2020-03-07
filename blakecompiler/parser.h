//  Copyright © 2020 Blake Lucchesi. All rights reserved.

#ifndef parser_h
#define parser_h

#include <stdio.h>
#include "lexer.h"

// Details about each item in the tree used for error reporting
typedef struct _ast_details {
    int line;
    int start;
} ASTDetails;

typedef struct _ast_identifier {
    char *name;
    ASTDetails details;
} ASTIdentifier;

typedef struct _ast_unary_operator {
    char *value;
    ASTDetails details;
} ASTUnaryOperator;

typedef struct _ast_expression {
    char *value;
    ASTUnaryOperator *unary_operator;
    struct _ast_expression *expression;
    ASTDetails details;
} ASTExpression;

typedef struct _ast_return {
    ASTExpression *expression;
    ASTDetails details;
} ASTReturn;

typedef struct _ast_statement {
    ASTExpression *expression;
    ASTDetails details;
} ASTStatement;

typedef struct _ast_function {
    ASTIdentifier *identifier;
    ASTStatement *statement;
    ASTDetails details;
} ASTFunction;

typedef struct _ast_comment {
    char *value;
    ASTDetails details;
} ASTComment;

typedef struct _ast_error {
    Token *token;
    char *message;
    struct _ast_error *next;
} ASTError;

typedef struct _ast_program {
    ASTFunction *function;
    ASTDetails details;
    ASTError *errors;
} ASTProgram;

ASTProgram *ASTParse(Token **start);

#endif /* parser_h */
