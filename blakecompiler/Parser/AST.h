//  Copyright © 2020 Blake Lucchesi. All rights reserved.

#ifndef AST_h
#define AST_h

#include <stdio.h>
#include "Token.h"

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
//    TokenName tokenName; // TODO switch on tokens not char values
    ASTUnaryOperator *unary_operator;
    struct _ast_expression *expression;
    ASTDetails details;
} ASTExpression;

typedef struct _ast_binary_operator {
    char *value;
    ASTExpression *expression_left;
    ASTExpression *expression_right;
} ASTBinaryOperator;

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


#endif /* AST_h */
