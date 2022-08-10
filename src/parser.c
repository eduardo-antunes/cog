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

// Error reporting functions --------------------------------------

static void error_at(Token *tok, const char *msg)
{
    eprintf("Error at ");
    switch(tok->type) {
        case TOKEN_END:
            eprintf("end: %s\n", msg);
            break;
        default:
            eprintf("line %d: %s\n", tok->line, msg);
    }
}

static void error(Parser *pr, const char *format, ...)
{
    // Ignore errors when panicking
    if(pr->panic) return;

    // Format error message
    char msg[32];
    va_list args;
    va_start(args, format);
    vsnprintf(msg, 32, format, args);
    va_end(args);

    // Report error and set error flags
    error_at(&pr->current, msg);
    pr->had_error = true;
    pr->panic = true;
}

// Core functions -------------------------------------------------

static void advance(Parser *pr)
{
    pr->prev = pr->current;

    while(true) {
        pr->current = get_token(&pr->lex);
        if(pr->current.type != TOKEN_ERR) break;
        error(pr, pr->current.start);
    }
}

static bool match(Parser *pr, Token_t type)
{
    if(pr->current.type == type) {
        advance(pr);
        return true;
    }
    return false;
}

static void expect(Parser *pr, Token_t type)
{
    if(!match(pr, type)) {
        error(pr, "expected token of type %d", type);
    }
}

// Parsing functions ----------------------------------------------

// Just a plain old recursive descent algorithm.

static void disjunction(Parser *pr, Box *box);

static void expression(Parser *pr, Box *box)
{
    disjunction(pr, box);
}

static void primary(Parser *pr, Box *box)
{
    switch(pr->current.type) {
        case TOKEN_NUM:
            advance(pr);
            double val = strtod(pr->prev.start, NULL);
            uint8_t i = box_value_write(box, val);
            box_code_write(box, OP_PUSH);
            box_code_write(box, i);
            break;
        case TOKEN_TRUE:
            advance(pr);
            box_code_write(box, OP_TRUE);
            break;
        case TOKEN_FALSE:
            advance(pr);
            box_code_write(box, OP_FALSE);
            break;
        case TOKEN_OPEN_PAREN:
            advance(pr);
            expression(pr, box);
            expect(pr, TOKEN_CLOSE_PAREN);
            break;
        default:
            error(pr, "unexpected EOF");
    }
}

static void unary(Parser *pr, Box *box)
{
    if(match(pr, TOKEN_MINUS)) {
        unary(pr, box);
        box_code_write(box, OP_NEG);
        return;
    }

    if(match(pr, TOKEN_NOT)) {
        unary(pr, box);
        box_code_write(box, OP_NOT);
        return;
    }

    primary(pr, box);
}

static void factor(Parser *pr, Box *box)
{
    // Factor
    unary(pr, box);
    Op_code op;
    while(match(pr, TOKEN_STAR) || match(pr, TOKEN_SLASH)) {
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
        unary(pr, box);
        box_code_write(box, op);
    }
}

static void term(Parser *pr, Box *box)
{
    // Term
    factor(pr, box);
    Op_code op;
    while(match(pr, TOKEN_PLUS) || match(pr, TOKEN_MINUS)) {
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
        factor(pr, box);
        box_code_write(box, op);
    }
}

static void conjunction(Parser *pr, Box *box)
{
    term(pr, box);
    while(match(pr, TOKEN_AND)) {
        term(pr, box);
        box_code_write(box, OP_AND);
    }
}

static void disjunction(Parser *pr, Box *box)
{
    conjunction(pr, box);
    while(match(pr, TOKEN_OR)) {
        conjunction(pr, box);
        box_code_write(box, OP_OR);
    }
}

// Public interface -----------------------------------------------

void parser_init(Parser *pr, const char *source)
{
    pr->panic = false;
    pr->had_error = false;
    lexer_init(&pr->lex, source);
}

bool parse(Parser *pr, Box *box)
{
    advance(pr);
    expression(pr, box);
    return !pr->had_error;
}
