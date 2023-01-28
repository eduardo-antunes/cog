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

int cog_array_init(Cog_array *arr, int initial_capacity) {
    if(initial_capacity <= 0) 
        initial_capacity = COG_ARRAY_INITIAL_CAPACITY;

    arr->count = 0;
    arr->capacity = initial_capacity;
    arr->values = (Cog_value*) malloc(arr->capacity * sizeof(Cog_value));
    if(arr->values == NULL) return 1;
    return 0;
}

int cog_array_push(Cog_array *arr, Cog_value value) {
    if(arr->count + 1 > arr->capacity) {
        arr->capacity *= COG_ARRAY_GROWTH_FACTOR;
        Cog_value *ptr = realloc(arr->values, arr->capacity);
        if(ptr == NULL)
            return 1;
        arr->values = ptr;
    }
    int index = arr->count++;
    arr->values[index] = value;
    return index;
}

Cog_value cog_array_get(const Cog_array *arr, int index) {
    if(index < 0 || index >= arr->count)
        return NONE_VALUE;
    return arr->values[index];
}

Cog_value cog_array_pop(Cog_array *arr) {
    if(arr->count == 0)
        return NONE_VALUE;
    return arr->values[--arr->count];
}

void cog_array_free(Cog_array *arr) {
    free(arr->values);
    arr->capacity = 0;
    arr->count = 0;
}
