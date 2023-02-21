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

static bool is_space(char ch) {
    return ch == ' '
        || ch == '\n'
        || ch == '\r'
        || ch == '\t'
        || ch == '\v';
}

static bool is_digit(char ch) {
    return ch >= '0' && ch <= '9';
}

// Checks whether the character is a valid first character of an indentifier
static bool is_valid_first(char ch) {
    return (ch >= 'a' && ch <= 'z')
        || (ch >= 'A' && ch <= 'Z')
        || ch == '_';
}

// Checks whether the chacter is a valid non-first character of an identifier
static bool is_valid_rest(char ch) {
    return is_valid_first(ch)
        || is_digit(ch)
        || ch == '!'
        || ch == '?';
}

static char advance(Lexer *lex) {
    ++lex->current;
    return lex->current[-1];
}

static char peek(const Lexer *lex) {
    return lex->current[0];
}

static bool at_end(const Lexer *lex) {
    return lex->current[0] == END;
}

static bool match(Lexer *lex, char ch) {
    if(lex->current[0] == ch) {
        ++lex->current;
        return true;
    }
    return false;
}

// Lexing functions

// Most general token creation function
static Token make_token(Lexer *lex, Token_t type) {
    Token tok;
    tok.type = type;
    tok.start = lex->start;
    tok.offset = lex->current - lex->start;
    tok.line = lex->line;
    return tok;
}

// Report errors by emiting tokens
static Token error_token(Lexer *lex, const char *text) {
    Token err;
    err.type = TOKEN_ERR;
    err.start = text;
    err.offset = strlen(text);
    err.line = lex->line;
    return err;
}

static Token number_token(Lexer *lex) {
    // Integer part
    while(is_digit(peek(lex)))
        advance(lex);
    // Optional decimal part
    if(match(lex, '.')) {
        while(is_digit(peek(lex)))
            advance(lex);
    }
    return make_token(lex, TOKEN_NUM);
}

static Token symbol_token(Lexer *lex) {
    // We've already consumed the double colon
    // It should not be included in the symbol, so:
    ++lex->start;
    // Now we essentially lex an identifier
    if(is_valid_first(peek(lex))) {
        advance(lex);
        while(is_valid_rest(peek(lex)))
            advance(lex);
        return make_token(lex, TOKEN_SYM);
    }
    return error_token(lex, "Invalid symbol");
}

static Token id_or_keyword_token(Lexer *lex) {
    // We've already consumed the first character
    // Now we consume the rest...
    while(is_valid_rest(peek(lex)))
        advance(lex);
    // ...and then check if what we've got is an
    // identifier or a keyword
    switch(lex->start[0]) {
        case 'a':
            // Can be 'and'
            if(lex->current - lex->start == 3
                    && lex->start[1] == 'n'
                    && lex->start[2] == 'd')
                return make_token(lex, TOKEN_AND);
            break;
        case 'f':
            // Can be 'false'
            if(lex->current - lex->start == 5
                    && lex->start[1] == 'a'
                    && lex->start[2] == 'l'
                    && lex->start[3] == 's'
                    && lex->start[4] == 'e')
                return make_token(lex, TOKEN_FALSE);
            break;
        case 'n':
            // Can be 'not'...
            if(lex->current - lex->start == 3
                    && lex->start[1] == 'o'
                    && lex->start[2] == 't')
                return make_token(lex, TOKEN_NOT);
            // ...or 'none'
            else if(lex->current - lex->start == 4
                    && lex->start[1] == 'o'
                    && lex->start[2] == 'n'
                    && lex->start[3] == 'e')
                return make_token(lex, TOKEN_NONE);
            break;
        case 't':
            // Can be 'true'
            if(lex->current - lex->start == 4
                    && lex->start[1] == 'r'
                    && lex->start[2] == 'u'
                    && lex->start[3] == 'e')
                return make_token(lex, TOKEN_TRUE);
            break;
    }

    // Can only be an identifier
    return error_token(lex, "Variables are currently not supported");
}

static void skip_whitespace(Lexer *lex) {
    char ch;
    while(is_space(peek(lex)) || peek(lex) == '#') {
        ch = advance(lex);
        switch(ch) {
            case '\n':
                ++lex->line;
                break;
            case '#':
                // Python-style comments
                while(peek(lex) != '\n' && !at_end(lex))
                    advance(lex);
                break;
        }
    }
}

// Public interface

void lexer_init(Lexer *lex, const char *source) {
    lex->start = lex->current = source;
    lex->line = 1;
}

Token lexer_get_token(Lexer *lex) {
    skip_whitespace(lex);
    lex->start = lex->current;

    char ch = advance(lex);
    // Tokens of small length
    switch(ch) {
        // Single character tokens
        case '+': return make_token(lex, TOKEN_PLUS);
        case '-': return make_token(lex, TOKEN_MINUS);
        case '*': return make_token(lex, TOKEN_STAR);
        case '/': return make_token(lex, TOKEN_SLASH);
        case '(': return make_token(lex, TOKEN_OPEN_PAREN);
        case ')': return make_token(lex, TOKEN_CLOSE_PAREN);
        case END: return make_token(lex, TOKEN_END);

        // Possibly double character tokens
        case '<':
            if(match(lex, '='))
                return make_token(lex, TOKEN_LESS_EQUAL);
            return make_token(lex, TOKEN_LESS);

        case '>':
            if(match(lex, '='))
                return make_token(lex, TOKEN_GREATER_EQUAL);
            return make_token(lex, TOKEN_GREATER);

        case '=':
            if(match(lex, '='))
                return make_token(lex, TOKEN_EQUAL_EQUAL);
            return make_token(lex, TOKEN_EQUAL);

        case '!':
            if(match(lex, '='))
                return make_token(lex, TOKEN_NOT_EQUAL);
            return error_token(lex, "Unexpected '!'; do you mean 'not'?");

    }

    if(is_digit(ch)) 
        // It can only be a number
        return number_token(lex);

    if(ch == ':')
        // It can only be a symbol
        return symbol_token(lex);

    if(is_valid_first(ch))
        // It can be an identifier or a keyword
        return id_or_keyword_token(lex);

    return error_token(lex, "Unrecognized character!");
}
