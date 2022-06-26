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

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "common.h"
#include "parser.h"
#include "lexer.h"

static void error(Token *tok, const char *msg)
{
    eprintf("[line %d] Error", tok->line);
    switch(tok->type) {
        case TOKEN_END:
            eprintf(" at end");
            break;
        case TOKEN_ERR:
            // Nothing
            break;
        default:
            eprintf(" at '%.*s'", tok->offset, tok->start);
    }
    eprintf(": %s\n", msg);
}

static void error_current(Parser *pr, const char *msg)
{
    // Ignore errors when panicking
    if(pr->panic) return;

    error(&pr->current, msg);
    pr->had_error = true;
    pr->panic = true;
}

static void error_prev(Parser *pr, const char *msg)
{
    // Ignore errors when panicking
    if(pr->panic) return;

    error(&pr->prev, msg);
    pr->had_error = true;
    pr->panic = true;
}

static void advance(Parser *pr)
{
    pr->prev = pr->current;

    while(true) {
        pr->current = get_token(&pr->lex);
        if(pr->current.type != TOKEN_ERR) break;
        error_current(pr, pr->current.start);
    }
}

static bool accept(Parser *pr, Token_t type)
{
    if(pr->current.type == type) {
        advance(pr);
        return true;
    }
    return false;
}

static void expect(Parser *pr, Token_t type)
{
    // TODO improve message
    if(!accept(pr, type))
        error_current(pr, "expected something else!\n");
}

// Initialize parser
void init_parser(Parser *pr, const char *source)
{
    pr->panic = false;
    pr->had_error = false;
    init_lexer(&pr->lex, source);
}

// Recursive descent parser: yes!

static void term(Parser *pr);

static void expression(Parser *pr)
{
    // Parses a whole expression
    term(pr);
}

static void primary(Parser *pr)
{
    // Primary thingy
    if(accept(pr, TOKEN_NUM)) {
        // Just a number
    } else {
        // Parenthesized expression
        expect(pr, TOKEN_OPEN_PAREN);
        expression(pr);
        expect(pr, TOKEN_CLOSE_PAREN);
    }
}

static void unary(Parser *pr)
{
    // Unary negation
    if(accept(pr, TOKEN_SUB)) {
        printf("-");
        unary(pr);
        return;
    }
    primary(pr);
}

static void factor(Parser *pr)
{
    // Factor
    unary(pr);
    while(accept(pr, TOKEN_MUL) || accept(pr, TOKEN_DIV)) {
        switch(pr->prev.type) {
            case TOKEN_MUL:
                printf("*");
                break;
            case TOKEN_DIV:
                printf("/");
                break;
            default:
                break;
        }
        unary(pr);
    }
}

static void term(Parser *pr)
{
    // Term
    factor(pr);
    while(accept(pr, TOKEN_ADD) || accept(pr, TOKEN_SUB)) {
        switch(pr->prev.type) {
            case TOKEN_ADD:
                printf("+");
                break;
            case TOKEN_SUB:
                printf("-");
                break;
            default:
                break;
        }
        factor(pr);
    }
}

bool parse(Parser *pr)
{
    advance(pr);
    expression(pr);
    accept(pr, TOKEN_END);
    printf("\n");
    return !pr->had_error;
}
