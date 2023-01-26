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

// Cog values and data structures, featuring dynamic typing

#ifndef COG_VALUE_H
#define COG_VALUE_H

#include "common.h"

typedef enum {
    TYPE_NUMBER,
    TYPE_BOOLEAN,
    TYPE_NONE,
} Cog_type;

typedef struct {
    union {
        double number;
        bool boolean;
    } as;
    Cog_type type;
} Cog_value;

void cog_value_print(Cog_value val);

// Conversion of C values to Cog values

#define COG_CAST(type, expr) ((Cog_value) { {(expr)}, (type) })

#define COG_NUM(n)  ((Cog_value) { {.number = (n)},  TYPE_NUMBER  })

#define COG_BOOL(b) ((Cog_value) { {.boolean = (b)}, TYPE_BOOLEAN })

#define COG_NONE    ((Cog_value) { {.number = 0},    TYPE_NONE    })

// Conversion of Cog values to C values

#define TO_NUM(v) ((v).as.number)

#define TO_BOOL(v) ((v).as.boolean)

#define LOGIC_VAL(v) \
    ((((v).type != TYPE_NONE) \
     && ((v).type != TYPE_BOOLEAN)) \
     || (TO_BOOL(v)))

// Type checking operations

#define IS_NUM(v) ((v).type == TYPE_NUMBER)

#define IS_BOOL(v) ((v).type == TYPE_BOOLEAN)

#define IS_NONE(v) ((v).type == TYPE_NONE)

// Basic dynamic array

#define COG_ARRAY_INITIAL_CAPACITY 10
#define COG_ARRAY_GROWTH_FACTOR 2

typedef struct {
    Cog_value *p;
    unsigned count;
    unsigned capacity;
} Cog_array;

void cog_array_init(Cog_array *arr);

void cog_array_push(Cog_array *arr, Cog_value val);

Cog_value cog_array_get(const Cog_array *arr, unsigned index);

Cog_value cog_array_pop(Cog_array *arr);

void cog_array_free(Cog_array *arr);

#endif // COG_VALUE_H
