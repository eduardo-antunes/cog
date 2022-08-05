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
#include <ctype.h>

#include "common.h"
#include "lexer.h"

#define END '\0'

static bool is_valid(char ch)
{
    return isalpha(ch) || ch == '_';
}

// Initialize the lexer
void lexer_init(Lexer *lex, const char *source)
{
    lex->start = source;
    lex->current = source;
    lex->line = 1;
}

// See the current character and move the
// lexer a character forward
static char advance(Lexer *lex)
{
    lex->current++;
    return lex->current[-1];
}

// See the current character
static char peek(Lexer *lex)
{
    return lex->current[0];
}

// Make a token using the lexer's current state
static Token mktok(Lexer *lex, Token_t t)
{
    Token tok;
    tok.start = lex->start;
    tok.offset = lex->current - lex->start;
    tok.line = lex->line;
    tok.type = t;
    return tok;
}

// Report an error as an error token
static Token lex_error(Lexer *lex, const char *why)
{
    Token err;
    err.type = TOKEN_ERR;
    err.line = lex->line;
    err.start = why;
    err.offset = strlen(why);
    return err;
}

// Tokenize a number, integer or floating-point
static Token number_tok(Lexer *lex)
{
    // Integer part
    while(isdigit(peek(lex))) advance(lex);

    // Optional decimal part
    if(peek(lex) == '.') {
        advance(lex);
        while(isdigit(peek(lex)))
            advance(lex);
    }

    return mktok(lex, TOKEN_NUM);
}

static bool check_keyword(Lexer *lex, const char *rest)
{
    int len = strlen(rest);
    for(int i = 1; i <= len; ++i) {
        if(lex->start[i] != rest[i + 1])
            return false;
    }
    return true;
}

// Decide whether a valid id is a keyword
static Token_t id_tok_type(Lexer *lex)
{
    bool keyword = false;
    switch(lex->start[0]) {
        case 'a':
            // and
            keyword = check_keyword(lex, "nd");
            if(keyword) return TOKEN_AND;
            break;
        case 'f':
            // false keyword
            keyword = check_keyword(lex, "alse");
            if(keyword) return TOKEN_FALSE;
            break;
        case 'o':
            // or
            keyword = check_keyword(lex, "r");
            if(keyword) return TOKEN_OR;
            break;
        case 'n':
            // not
            keyword = check_keyword(lex, "ot");
            if(keyword) return TOKEN_OR;
            break;
        case 't':
            // true keyword
            keyword = check_keyword(lex, "rue");
            if(keyword) return TOKEN_TRUE;
            break;
    }

    return TOKEN_ID;
}

// Tokenize an identifier/keyword
static Token id_tok(Lexer *lex)
{
    while(is_valid(peek(lex)) || isdigit(peek(lex))) 
        advance(lex);

    return mktok(lex, id_tok_type(lex));
}

// Ignore whitespace and comments
static void ignore_whitespace(Lexer *lex)
{
    char ch;
    while(isspace(peek(lex)) || peek(lex) == '#') {
        ch = advance(lex);
        switch(ch) {
            case '\n':
                lex->line++;
                break;
            case '\r':
                if(peek(lex) == '\n')
                    advance(lex);
                lex->line++;
                break;
            case '#':
                while(peek(lex) != '\n' && peek(lex) != '\r')
                    advance(lex);
                break;
        }
    }
}

// Yield a single token from source
Token get_token(Lexer *lex)
{
    ignore_whitespace(lex);
    lex->start = lex->current;

    char ch = advance(lex);

    switch(ch) {
        case '+': return mktok(lex, TOKEN_PLUS);
        case '-': return mktok(lex, TOKEN_MINUS);
        case '*': return mktok(lex, TOKEN_STAR);
        case '/': return mktok(lex, TOKEN_SLASH);
        case '(': return mktok(lex, TOKEN_OPEN_PAREN);
        case ')': return mktok(lex, TOKEN_CLOSE_PAREN);
        case END: return mktok(lex, TOKEN_END);
    }

    if(is_valid(ch)) 
        return id_tok(lex);

    else if(isdigit(ch)) 
        return number_tok(lex);

    return lex_error(lex, "Unrecognized char");
}
