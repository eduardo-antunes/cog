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

#include <stdlib.h>

#include "common.h"
#include "vector.h"

void vector_init(Vector *v)
{
    v->tokens = malloc(sizeof(Token) * INITIAL_CAPACITY);
    v->capacity = INITIAL_CAPACITY;
    v->count = 0;
}

void vector_push(Vector *v, Token tok)
{
    if(v->count + 1 > v->capacity) {
        v->capacity *= GROWTH_FACTOR;
        v->tokens = realloc(v->tokens, v->capacity);
    }
    v->tokens[v->count++] = tok;
}

void vector_free(Vector *v)
{
    free(v->tokens);
    v->capacity = 0;
    v->count = 0;
}
