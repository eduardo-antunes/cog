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
#include "box.h"

static Box *box;

static void report_error(Token *tok, const char *msg)
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

static void error(Parser *pr, const char *msg)
{
    // Ignore errors when panicking
    if(pr->panic) return;

    report_error(&pr->current, msg);
    pr->had_error = true;
    pr->panic = true;
}

static void advance(Parser *pr)
{
    pr->prev = pr->current;

    while(true) {
        pr->current = get_token(&pr->lex);
        if(pr->current.type != TOKEN_ERR) break;
        error(pr, pr->current.start);
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
    if(!accept(pr, type)) {
        char error_msg[32];
        sprintf(error_msg, "expected token of type %d", type);
        error(pr, error_msg);
    }
}

// Initialize parser
void parser_init(Parser *pr, const char *source)
{
    pr->panic = false;
    pr->had_error = false;
    lexer_init(&pr->lex, source);
}

// Recursive descent parser: yes!

static void term(Parser *pr);

static void expression(Parser *pr)
{
    // Parses a whole expression
    term(pr);
}

static void atom(Parser *pr)
{
    // Primary thingy
    if(accept(pr, TOKEN_NUM)) {
        // Just a number
        double val = strtod(pr->prev.start, NULL);
        uint8_t i = box_value_write(box, val);
        box_code_write(box, OP_PUSH);
        box_code_write(box, i);
    } else if(accept(pr, TOKEN_OPEN_PAREN)) {
        // Parenthesized expression
        expression(pr);
        expect(pr, TOKEN_CLOSE_PAREN);
    } else {
        error(pr, "unexpected EOF");
        return;
    }
}

static void unary(Parser *pr)
{
    // Unary negation
    if(accept(pr, TOKEN_MINUS)) {
        unary(pr);
        box_code_write(box, OP_NEG);
        return;
    }
    atom(pr);
}

static void factor(Parser *pr)
{
    // Factor
    unary(pr);
    Op_code op;
    while(accept(pr, TOKEN_STAR) || accept(pr, TOKEN_SLASH)) {
        switch(pr->prev.type) {
            case TOKEN_STAR:
                op = OP_MUL;
                break;
            case TOKEN_SLASH:
                op = OP_DIV;
                break;
            default:
                break;
        }
        unary(pr);
        box_code_write(box, op);
    }
}

static void term(Parser *pr)
{
    // Term
    factor(pr);
    Op_code op;
    while(accept(pr, TOKEN_PLUS) || accept(pr, TOKEN_MINUS)) {
        switch(pr->prev.type) {
            case TOKEN_PLUS:
                op = OP_ADD;
                break;
            case TOKEN_MINUS:
                op = OP_SUB;
                break;
            default:
                break;
        }
        factor(pr);
        box_code_write(box, op);
    }
}

bool parse(Parser *pr, Box *b)
{
    box = b;
    advance(pr);
    expression(pr);
    return !pr->had_error;
}
