//  Copyright © 2020 Blake Lucchesi. All rights reserved.

#include "parser.h"

// types:
//program = Program(function_declaration)
//function_declaration = Function(string, statement) //string is the function name
//statement = Return(exp)
//exp = Constant(int)


// psuedo code
//def parse_statement(tokens):
//tok = tokens.next()
//if tok.type != "RETURN_KEYWORD":
//    fail()
//tok = tokens.next()
//if tok.type != "INT"
//    fail()
//exp = parse_exp(tokens) //parse_exp will pop off more tokens
//statement = Return(exp)
//
//tok = tokens.next()
//if tok.type != "SEMICOLON":
//    fail()
//
//return statement
