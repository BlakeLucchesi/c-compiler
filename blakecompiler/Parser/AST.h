//  Copyright © 2020 Blake Lucchesi. All rights reserved.

#ifndef AST_h
#define AST_h

#include <stdio.h>
#include "Token.h"

struct _ast_expression;

typedef struct _ast_details {
    int line;
    int start;
} ASTDetails;

typedef struct _ast_identifier {
    char *name;
    ASTDetails details;
} ASTIdentifier;

typedef struct _ast_unary_operator {
    ASTDetails details;
    char *op;
    struct _ast_expression *expression;
} ASTUnaryOperator;

typedef struct _ast_binary_operator {
    ASTDetails details;
    char *op;
    struct _ast_expression *lhs;
    struct _ast_expression *rhs;
} ASTBinaryOperator;

typedef struct _ast_expression {
    ASTDetails details;
    char *value;
    ASTUnaryOperator *unary_op;
    ASTBinaryOperator *binary_op;
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


#endif /* AST_h */
