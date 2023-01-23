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
#include "value.h"

void vector_init(Cog_vector *v) {
    v->val = (Cog_val*) malloc(sizeof(Cog_val) *
            COG_VECTOR_INITIAL_CAPACITY);
    v->capacity = COG_VECTOR_INITIAL_CAPACITY;
    v->count = 0;
}

void vector_push(Cog_vector *v, Cog_val val) {
    if(v->count + 1 < v->capacity) {
        v->capacity *= COG_VECTOR_GROWTH_FACTOR;
        v->val = realloc(v->val, v->capacity);
    }
    v->val[v->count] = val;
    v->count++;
}

Cog_val vector_get(const Cog_vector *v, unsigned int index) {
    if(index >= v->count) {
        // TODO: signal an error somehow
        return 0;
    }
    return v->val[index];
}

Cog_val vector_pop(Cog_vector *v) {
    return v->val[--v->count];
}

void vector_free(Cog_vector *v) {
    free(v->val);
    v->capacity = 0;
    v->count = 0;
}
