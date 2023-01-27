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

int box_init(Box *box) {
    box->code = (uint8_t*) malloc(BOX_CODE_INITIAL_CAPACITY * sizeof(uint8_t));
    if(box->code == NULL) return 1;
    int err = cog_array_init(&box->constants);
    if(err) return 2;

    box->capacity = BOX_CODE_INITIAL_CAPACITY;
    box->count = 0;
    return 0;
}

int box_code_write(Box *box, uint8_t byte) {
    if(box->count + 1 > box->capacity) {
        box->capacity *= BOX_CODE_GROWTH_FACTOR;
        uint8_t *ptr = realloc(box->code, box->capacity);
        if(ptr == NULL)
            return 1;
        box->code = ptr;
    }
    box->code[box->count++] = byte;
    return 0;
}

uint8_t box_value_write(Box *box, Cog_value value) {
    // this is an unsafe cast to uint8_t (fine by now)
    uint8_t index = cog_array_push(&box->constants, value);
    return index;
}

void box_free(Box *box) {
    free(box->code);
    box->capacity = 0;
    box->count = 0;
}
