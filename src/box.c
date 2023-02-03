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

#include "box.h"
#include "common.h"
#include "memory.h"

void box_init(Box *box) {
    box->code = (uint8_t*) cog_realloc(NULL, 0, BOX_CODE_INITIAL_CAPACITY * sizeof(uint8_t));
    cog_array_init(&box->constants, -1);
    box->capacity = BOX_CODE_INITIAL_CAPACITY;
    box->count = 0;
}

void box_code_write(Box *box, uint8_t byte) {
    if(box->count + 1 > box->capacity) {
        int new_capacity = box->capacity * BOX_CODE_GROWTH_FACTOR;
        box->code = cog_realloc(box->code, box->capacity, new_capacity);
    }
    box->code[box->count++] = byte;
}

uint8_t box_value_write(Box *box, Cog_value value) {
    int index = cog_array_push(&box->constants, value);
    // NOTE this cast is unsafe, but works for now
    return (uint8_t) index;
}

void box_free(Box *box) {
    free(box->code);
    box->capacity = 0;
    box->count = 0;
}
