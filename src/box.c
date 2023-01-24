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
#include "value.h"

void box_init(Box *b) {
    b->count = 0;
    b->capacity = BOX_CODE_INITIAL_CAPACITY;
    b->code = (uint8_t*) malloc(sizeof(uint8_t) *
            BOX_CODE_INITIAL_CAPACITY);
    cog_array_init(&b->constants);
}

void box_code_write(Box *b, uint8_t byte) {
    // TODO Really gotta improve this
    if(b->count + 1 > b->capacity) {
        b->capacity *= BOX_CODE_GROWTH_FACTOR;
        b->code = realloc(b->code, b->capacity);
    }
    b->code[b->count++] = byte;
}

uint8_t box_value_write(Box *b, Cog_value val) {
    cog_array_push(&b->constants, val);
    return b->constants.count - 1;
}

void box_free(Box *b) {
    free(b->code);
    cog_array_free(&b->constants);
    b->capacity = b->count = 0;
}
