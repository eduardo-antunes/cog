/*
   Copyright 2022 Eduardo Antunes dos Santos Vieira

   This file is part of cog.

   cog is free software: you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation,
   either version 3 of the License, or (at your option)
   any later version.

   cog is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied
   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
   PURPOSE. See the GNU General Public License for more
   details.

   You should have received a copy of the GNU General
   Public License along with cog. If not, see
   <https://www.gnu.org/licenses/>.
*/

#include <string.h>

#include "common.h"
#include "lexer.h"

// Core functions and definitions

#define END '\0'

static bool is_alpha(char ch) {
    return (ch >= 'a' && ch <= 'z') 
        || (ch >= 'A' && ch <= 'Z')
        || ch == '_';
}

static bool is_digit(char ch) {
    return ch >= '0' && ch <= '9';
}

static bool is_space(char ch) {
    return ch == ' ' 
        || ch == '\n' 
        || ch == '\r' 
        || ch == '\t' 
        || ch == '\v';
}

static char advance(Lexer *lex) {
    ++lex->current;
    return lex->current[-1];
}

static char peek(Lexer *lex) {
    return lex->current[0];
}

static char peek_next(Lexer *lex) {
    return lex->current[1];
}

// Lexing functions

static Token make_tok(Lexer *lex, Token_t type) {
    Token tok;
    tok.type = type;
    tok.start = lex->start;
    tok.offset = lex->current - lex->start;
    tok.line = lex->line;
    return tok;
}

static Token make_err_tok(Lexer *lex, const char *text) {
    Token err;
    err.type = TOKEN_ERR;
    err.start = text;
    err.offset = strlen(text);
    err.line = lex->line;
    return err;
}

static Token make_num_tok(Lexer *lex) {
    while(is_digit(peek(lex))) // integer part
        advance(lex);

    if(peek(lex) == '.') { // fractional part
        advance(lex);
        while(is_digit(peek(lex)))
            advance(lex);
    }

    return make_tok(lex, TOKEN_NUM);
}

static Token_t check_keyword(Lexer *lex, int start, 
        int length, const char *rest, Token_t type) {
    // Credits for this function go to Bob Nystrom.
    // This is just so straightforward, I had to borrow it.
    if(lex->current - lex->start == start + length &&
            memcmp(lex->start + start, rest, length) == 0)
        return type;
    return TOKEN_ID;
}

static Token_t id_type(Lexer *lex) {
    switch(lex->start[0]) {
        case 'a': return check_keyword(lex, 1, 2, "nd",   TOKEN_AND);
        case 'f': return check_keyword(lex, 1, 4, "alse", TOKEN_FALSE);
        case 'o': return check_keyword(lex, 1, 1, "r",    TOKEN_OR);
        case 'n': return check_keyword(lex, 1, 2, "ot",   TOKEN_NOT);
        case 't': return check_keyword(lex, 1, 3, "rue",  TOKEN_TRUE);
    }
    return TOKEN_ID;
}

static Token make_id_tok(Lexer *lex) {
    // here we've already consumed the first character
    while(is_alpha(peek(lex)) || is_digit(peek(lex))) 
        advance(lex);
    // the identifier might be a keyword
    return make_tok(lex, id_type(lex));
}

static void skip_whitespace(Lexer *lex) {
    char ch;
    while(is_space(peek(lex)) || peek(lex) == '#') {
        ch = advance(lex);
        switch(ch) {
            case '\n':
                ++lex->line;
                break;
            case '\r':
                if(peek(lex) == '\n')
                    advance(lex);
                ++lex->line;
                break;
            case '#':
                while(peek(lex) != '\n' 
                        || (peek(lex) != '\r' && peek_next(lex) != '\n'))
                    advance(lex);
                break;
        }
    }
}

// Public interface

void lexer_init(Lexer *lex, const char *source) {
    lex->start = source;
    lex->current = source;
    lex->line = 1;
}

Token lexer_get_tok(Lexer *lex) {
    skip_whitespace(lex);
    lex->start = lex->current;
    char ch = advance(lex);

    switch(ch) {
        case '+': return make_tok(lex, TOKEN_PLUS);
        case '-': return make_tok(lex, TOKEN_MINUS);
        case '*': return make_tok(lex, TOKEN_STAR);
        case '/': return make_tok(lex, TOKEN_SLASH);
        case '(': return make_tok(lex, TOKEN_OPEN_PAREN);
        case ')': return make_tok(lex, TOKEN_CLOSE_PAREN);
        case END: return make_tok(lex, TOKEN_END);
    }

    if(is_alpha(ch)) return make_id_tok(lex);
    if(is_digit(ch)) return make_num_tok(lex);

    return make_err_tok(lex, "Unrecognized char");
}
