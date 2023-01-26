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

#include "common.h"
#include "value.h"

typedef enum {
    OP_NEG,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,

    OP_NOT,
    OP_AND,
    OP_OR,

    OP_PSH,
    OP_PSH_TRUE,
    OP_PSH_FALSE,
    OP_PSH_NONE,

    OP_RET,
} Op_code;

#define BOX_CODE_INITIAL_CAPACITY 10
#define BOX_CODE_GROWTH_FACTOR 2

typedef struct {
    uint8_t *code;
    unsigned count;
    unsigned capacity;
    Cog_array constants;
} Box;

void box_init(Box *b);

void box_code_write(Box *b, uint8_t byte);

uint8_t box_value_write(Box *b, Cog_value val);

void box_free(Box *b);

#endif // COG_BOX_H
