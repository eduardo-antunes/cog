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

#ifndef COG_ARRAY_H
#define COG_ARRAY_H

#include "common.h"
#include "value.h"

#define COG_ARRAY_INITIAL_CAPACITY 8
#define COG_ARRAY_GROWTH_FACTOR 2

typedef struct {
    Cog_value *data;
    int count;
    int capacity;
} Cog_array;

void cog_array_init(Cog_array *arr, int initial_capacity);

int cog_array_push(Cog_array *arr, Cog_value value);

Cog_value cog_array_get(const Cog_array *arr, int index);

Cog_value cog_array_pop(Cog_array *arr);

void cog_array_free(Cog_array *arr);

#endif // COG_ARRAY_H
