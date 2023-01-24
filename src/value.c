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

#include "common.h"
#include "value.h"

void cog_value_print(Cog_value val) {
    switch(val.type) {
        case COG_NUMBER:
            printf("%g", TO_NUM(val));
            break;
        case COG_BOOLEAN:
            printf(TO_BOOL(val) ? "true" : "false");
            break;
        case COG_NULL:
            printf("null");
            break;
    }
}

void cog_array_init(Cog_array *arr) {
    arr->p = (Cog_value*) malloc(sizeof(Cog_value) *
            COG_ARRAY_INITIAL_CAPACITY);
    arr->capacity = COG_ARRAY_INITIAL_CAPACITY;
    arr->count = 0;
}

void cog_array_push(Cog_array *arr, Cog_value val) {
    if(arr->count + 1 < arr->capacity) {
        arr->capacity *= COG_ARRAY_GROWTH_FACTOR;
        arr->p = realloc(arr->p, arr->capacity);
    }
    arr->p[arr->count] = val;
    arr->count++;
}

Cog_value cog_array_get(const Cog_array *arr, unsigned index) {
    if(index >= arr->count)
        // TODO: signal an error
        return COG_NUM(0);
    return arr->p[index];
}

Cog_value cog_array_pop(Cog_array *arr) {
    if(arr->count == 0)
        // TODO signal an error
        return COG_NUM(0);
    return arr->p[--arr->count];
}

void cog_array_free(Cog_array *arr) {
    free(arr->p);
    arr->capacity = 0;
    arr->count = 0;
}
