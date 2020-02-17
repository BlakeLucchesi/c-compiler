//  Copyright © 2020 Blake Lucchesi. All rights reserved.

#ifndef parser_h
#define parser_h

#include <stdio.h>
#include "lexer.h"

// Details about each item in the tree used for error reporting
typedef struct _ast_details {
    int line;
    int start;
} AST_Details;

typedef struct _ast_identifier {
    char *name;
    AST_Details details;
} AST_Identifier;

typedef struct _ast_expression {
    char *value;
    AST_Details details;
} AST_Expression;

//typedef struct _ast_return {
//    AST_Expression *expression;
//    AST_Details details;
//} AST_Return;

typedef struct _ast_statement {
    AST_Expression *expression;
    AST_Details details;
} AST_Statement;

typedef struct _ast_function {
    AST_Identifier *identifier;
    AST_Statement *statement;
    AST_Details details;
} AST_Function;

typedef struct _ast_program {
    AST_Function *function;
    AST_Details details;
} AST_Program;

//typedef struct _ast_comment {
//
// AST_Comment}

AST_Program *parse(token **start);

#endif /* parser_h */
