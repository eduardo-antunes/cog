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

// Parsing component for cog

#ifndef COG_PARSER_H
#define COG_PARSER_H

#include "box.h"
#include "common.h"
#include "lexer.h"

typedef struct {
    Token current;
    Token prev;
    Lexer lex;
    bool panic;
    bool had_error;
} Parser;

void parser_init(Parser *pr, const char *source);

bool parse(Parser *pr, Box *b);

#endif // COG_PARSER_H
