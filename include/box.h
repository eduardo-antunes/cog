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

// Everything that relates to bytecode

#ifndef COG_BOX_H
#define COG_BOX_H

#include "array.h"
#include "common.h"
#include "value.h"

#define BOX_CODE_INITIAL_CAPACITY 10
#define BOX_CODE_GROWTH_FACTOR 2

typedef struct {
    uint8_t *code;
    unsigned count;
    unsigned capacity;
    Cog_array constants;
} Box;

int box_init(Box *box);

int box_code_write(Box *box, uint8_t byte);

uint8_t box_value_write(Box *box, Cog_value value);

void box_free(Box *box);

#endif // COG_BOX_H
