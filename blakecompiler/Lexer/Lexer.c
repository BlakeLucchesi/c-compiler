//  Copyright © 2020 Blake Lucchesi. All rights reserved.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "Lexer.h"
#include "LexerMatchers.h"
#include "Keywords.h"
#include "Debug.h"

char take_next(LexerState *state);
char peek_next(LexerState *state);
void emit_token(LexerState *state, TokenClass klass, TokenName name);


LexerState *MakeLexer() {
    LexerState *state = calloc(1, sizeof(LexerState));
    state->head = (Token *)calloc(1, sizeof(Token));
    state->tail = state->head;
    state->index = 0;
    state->line_number = 1;
    state->col_number = 1;
    return state;
}

Token *Lex(LexerState *state) {
    char c;
    TokenClass token_class = UNDEFINED_TOKEN;
    TokenName token_name = UNDEFINED_TOKEN_NAME;
    while ((c = take_next(state)) != EOF) {
        token_class = UNDEFINED_TOKEN;
        token_name = UNDEFINED_TOKEN_NAME;
        if (c == '\n') {
            state->line_number++;
            state->col_number = 1;
        }
        if (c == '/') {
            if (peek_next(state) == '/') {
                while (peek_next(state) != '\n') {
                    take_next(state);
                }
                token_class = COMMENT;
            }
            else {
                token_class = OPERATOR;
                token_name = OP_DIVISOR;
            }
        }
        else if (c == ';') {
            token_class = SEPARATOR;
            token_name = SEP_SEMICOLON;
        }
        else if (c == '{') {
            token_class = SEPARATOR;
            token_name = SEP_BRACE_OPEN;
        }
        else if (c == '}') {
            token_class = SEPARATOR;
            token_name = SEP_BRACE_CLOSE;
        }
        else if (c == '(') {
            token_class = SEPARATOR;
            token_name = SEP_PAREN_OPEN;
        }
        else if (c == ')') {
            token_class = SEPARATOR;
            token_name = SEP_PAREN_CLOSE;
        }
        else if (c == '+') {
            if (peek_next(state) == '+') {
                take_next(state);
                token_class = OPERATOR;
                // token_name;
            }
            else {
                token_class = OPERATOR;
                token_name = OP_ADDITION;
            }
        }
        else if (c == '-') {
            token_class = OPERATOR;
            token_name = OP_NEGATION;
        }
        else if (c == '*') {
            token_class = OPERATOR;
            token_name = OP_MULTIPLICATION;
        }
        else if (c == '&') {
            token_class = OPERATOR;
            token_name = OP_BITWISE_AND;
        }
        else if (c == '|') {
            token_class = OPERATOR;
            token_name = OP_BITWISE_OR;
        }
        else if (c == '!') {
            token_class = OPERATOR;
            token_name = OP_LOGICAL_NEGATION;
        }
        else if (c == '~') {
            token_class = OPERATOR;
            token_name = OP_BITWISE_COMPLEMENT;
        }
        else if (is_number(c)) {
            while (is_number(peek_next(state))) {
                take_next(state);
            }
            token_class = LITERAL;
        }
        else if (is_identifier_character(c)) {
            while (is_identifier_character(peek_next(state))) {
                take_next(state);
            }
            state->buffer[state->index] = '\0';
            token_class = is_keyword(state->buffer) || is_type(state->buffer) ? KEYWORD : IDENTIFIER;
            token_name = KEYWORD_RETURN;
        }
        if (token_class != UNDEFINED_TOKEN)
            emit_token(state, token_class, token_name);
        state->index = 0;
    }
    fclose(state->input);
    return state->tail->next;
}

char take_next(LexerState *state) {
    state->buffer[state->index] = fgetc(state->input);
    state->col_number++;
    state->index++;
    if (state->index > 240)
        abort(); // buffer overflow
    return state->buffer[state->index - 1];
}
    
char peek_next(LexerState *state) {
    char next = fgetc(state->input);
    fseek(state->input, -1, SEEK_CUR);
    return next;
}

void emit_token(LexerState *state, TokenClass klass, TokenName name) {
    Token *tmp = (Token *)calloc(1, sizeof(Token));
    tmp->line_number = state->line_number;
    tmp->col_number = state->col_number - state->index;
    tmp->value = (char *)malloc((state->index + 1)* sizeof(char));
    strncpy(tmp->value, state->buffer, state->index);
    tmp->value[state->index] = '\0';
    tmp->klass = klass;
    tmp->name = name;
    state->head->next = tmp;
    state->index = 0;
    state->head = tmp;
}

void print_debug(Token *token) {
    debug("Ln %2d, Col %2d | %12s | %s ", token->line_number, token->col_number, friendly_token_name(token), token->value);
}

const char *friendly_token_name(Token *token) {
    switch (token->klass) {
        case UNDEFINED_TOKEN:
            return "UNDEFINED";
        case OPERATOR:
            return "Operator";
            //    case BITWISE_COMPLEMENT:
            //        return "~";
            //    case NEGATION:
            //        return "Negation";
            //    case LOGICAL_NEGATION:
            //        return "NOT";
        case SEPARATOR:
            return "Separator";
        case IDENTIFIER:
            return "Identifier";
        case LITERAL:
            return "Literal";
        case COMMENT:
            return "Comment";
        case KEYWORD:
            return "Keyword";
    }
    return "UNKNOWN";
}

void cleanup(Token *head) {
//    token *op = head;
//    while (op != NULL) {
//        free(op->value);
//        op = op->next;
//    }
}
