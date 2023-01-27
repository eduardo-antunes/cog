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

#include "array.h"
#include "common.h"
#include "value.h"

int cog_array_init(Cog_array *arr) {
    arr->count = 0;
    arr->capacity = COG_ARRAY_INITIAL_CAPACITY;
    arr->values = (Cog_value*) malloc(arr->capacity * sizeof(Cog_value));
    if(arr->values == NULL) return 1;
    return 0;
}

unsigned cog_array_push(Cog_array *arr, Cog_value value) {
    if(arr->count + 1 > arr->capacity) {
        arr->capacity *= COG_ARRAY_GROWTH_FACTOR;
        Cog_value *ptr = realloc(arr->values, arr->capacity);
        if(ptr == NULL)
            // TODO signal this error properly
            return 0;
        arr->values = ptr;
    }
    int index = arr->count++;
    arr->values[index] = value;
    return index;
}

Cog_value cog_array_get(const Cog_array *arr, unsigned int index) {
    return arr->values[index];
}

void cog_array_free(Cog_array *arr) {
    free(arr->values);
    arr->capacity = 0;
    arr->count = 0;
}
