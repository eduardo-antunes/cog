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

#ifndef COG_VALUE_H
#define COG_VALUE_H

#include "common.h"

typedef double Cog_val; // temp

#define COG_VECTOR_INITIAL_CAPACITY 10
#define COG_VECTOR_GROWTH_FACTOR 2

typedef struct {
    Cog_val *val;
    unsigned count;
    unsigned capacity;
} Cog_vector;

void vector_init(Cog_vector *v);

void vector_push(Cog_vector *v, Cog_val val);

Cog_val vector_pop(Cog_vector *v);

Cog_val vector_get(unsigned index);

void vector_free(Cog_vector *v);

#endif // COG_VALUE_H
