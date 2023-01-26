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

#include "box.h"
#include "common.h"
#include "lexer.h"
#include "parser.h"

// Error reporting

static void parse_error(Parser *pr, const char *format, ...) {
    if(pr->panic) return; // ignore errors on panic mode
    pr->panic = pr->had_error = true;
    // show error message
    eprintf("(!) ");
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    // provide location information
    if(pr->current.type == TOKEN_END)
        eprintf(" (END)\n");
    else
        eprintf(" (line %d)\n", pr->current.line);
}

// Core functions

static void advance(Parser *pr) {
    pr->prev = pr->current;
    // report bad tokens
    while(true) {
        pr->current = lexer_get_tok(&pr->lex);
        if(pr->current.type != TOKEN_ERR) break;
        parse_error(pr, pr->current.start);
    }
}

static bool match(Parser *pr, Token_t type) {
    if(pr->current.type == type) {
        advance(pr);
        return true;
    }
    return false;
}

static void expect(Parser *pr, Token_t type) {
    if(!match(pr, type))
        parse_error(pr, "expected token of type %d", type);
}

// Parsing functions

static void parse_disj(Parser *pr, Box *box);

static void parse_expr(Parser *pr, Box *box) { 
    parse_disj(pr, box); 
}

static void parse_value(Parser *pr, Box *box) {
    switch(pr->current.type) {
        case TOKEN_NUM:
            advance(pr);
            double val = strtod(pr->prev.start, NULL);
            uint8_t i = box_value_write(box, COG_NUM(val));
            box_code_write(box, OP_PSH);
            box_code_write(box, i);
            break;

        case TOKEN_TRUE:
            advance(pr);
            box_code_write(box, OP_PSH_TRUE);
            break;

        case TOKEN_FALSE:
            advance(pr);
            box_code_write(box, OP_PSH_FALSE);
            break;

        case TOKEN_NONE:
            advance(pr);
            box_code_write(box, OP_PSH_NONE);
            break;

        case TOKEN_OPEN_PAREN: // parenthesized expression
            advance(pr);
            parse_expr(pr, box);
            expect(pr, TOKEN_CLOSE_PAREN);
            break;

        default:
            parse_error(pr, "Missing operand");
    }
}

static void parse_unary(Parser *pr, Box *box) {
    switch(pr->current.type) {
        case TOKEN_MINUS:
            advance(pr);
            parse_unary(pr, box);
            box_code_write(box, OP_NEG);
            break;

        case TOKEN_NOT:
            advance(pr);
            parse_unary(pr, box);
            box_code_write(box, OP_NOT);
            break;

        default:
            parse_value(pr, box);
            break;
    }
}

static void parse_prod(Parser *pr, Box *box) {
    Op_code op;
    parse_unary(pr, box);
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
        parse_unary(pr, box);
        box_code_write(box, op);
    }
}

static void parse_sum(Parser *pr, Box *box) {
    Op_code op;
    parse_prod(pr, box);
    while(match(pr, TOKEN_PLUS) || match(pr, TOKEN_MINUS)) {
        switch(pr->prev.type) {
            case TOKEN_PLUS:
                op = OP_ADD;
                break;
            case TOKEN_MINUS:
                op = OP_SUB;
                break;
            default:
                // unreachable
                break;
        }
        parse_prod(pr, box);
        box_code_write(box, op);
    }
}

// <, >, >=, <=
static void parse_comparison(Parser *pr, Box *box) {
    Op_code op;
    bool negate = false;
    parse_sum(pr, box);
    while(match(pr, TOKEN_LESS) || match(pr, TOKEN_LESS_EQUAL) ||
            match(pr, TOKEN_GREATER) || match(pr, TOKEN_GREATER_EQUAL)) {
        switch(pr->prev.type) {
            case TOKEN_LESS:
                op = OP_LT;
                break;
            case TOKEN_LESS_EQUAL:
                // (a <= b) = not (a > b)
                op = OP_GT;
                negate = true;
                break;
            case TOKEN_GREATER:
                op = OP_GT;
                break;
            case TOKEN_GREATER_EQUAL:
                // (a >= b) = not (a < b)
                op = OP_LT;
                negate = true;
                break;
            default:
                // unreachable
                break;
        }
        parse_sum(pr, box);
        box_code_write(box, op);
        if(negate) box_code_write(box, OP_NOT);
    }
}

// ==, !=
static void parse_equality(Parser *pr, Box *box) {
    bool negate = false;
    parse_comparison(pr, box);
    while(match(pr, TOKEN_EQUAL_EQUAL) || match(pr, TOKEN_NOT_EQUAL)) {
        if(pr->prev.type == TOKEN_NOT_EQUAL)
            negate = true;
        parse_comparison(pr, box);
        box_code_write(box, OP_EQ);
        if(negate) box_code_write(box, OP_NOT);
    }
}

static void parse_conj(Parser *pr, Box *box) {
    parse_equality(pr, box);
    while(match(pr, TOKEN_AND)) {
        parse_equality(pr, box);
        box_code_write(box, OP_AND);
    }
}

static void parse_disj(Parser *pr, Box *box) {
    parse_conj(pr, box);
    while(match(pr, TOKEN_OR)) {
        parse_conj(pr, box);
        box_code_write(box, OP_OR);
    }
}

// Public interface

void parser_init(Parser *pr, const char *source) {
    pr->panic = false;
    pr->had_error = false;
    lexer_init(&pr->lex, source);
}

bool parse(Parser *pr, Box *box) {
    advance(pr);
    parse_expr(pr, box);
    if(pr->current.type != TOKEN_END)
        parse_error(pr, "Malformed expression");
    return !pr->had_error;
}
