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

#ifndef COG_VECTOR_H
#define COG_VECTOR_H

#include "lexer.h"

typedef struct {
    Token *tokens;
    unsigned capacity;
    unsigned count;
} Vector;

#define INITIAL_CAPACITY 8
#define GROWTH_FACTOR 2

void vector_init(Vector *v);

void vector_push(Vector *v, Token tok);

void vector_free(Vector *v);

#endif // COG_VECTOR_H
