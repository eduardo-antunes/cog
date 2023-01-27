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

void cog_value_print(Cog_value value);

bool cog_value_equal(Cog_value a, Cog_value b);

// type checks

#define VALUE_IS(value, t) ((value).type == (t))

#define IS_NUMBER(value) VALUE_IS((value), TYPE_NUMBER)

#define IS_BOOLEAN(value) VALUE_IS((value), TYPE_BOOLEAN)

#define IS_NONE(value) VALUE_IS((value), TYPE_NONE)

// conversion: C value -> Cog_value

#define NUMBER_VALUE(n) ((Cog_value) { {.number = (n)}, TYPE_NUMBER })

#define BOOLEAN_VALUE(b) ((Cog_value) { {.boolean = (b)}, TYPE_BOOLEAN })

#define NONE_VALUE ((Cog_value) { {.number = 0}, TYPE_NONE })

// conversion: Cog_value -> C value

#define TO_NUMBER(value) ((value).as.number)

#define TO_BOOLEAN(value) ((value).as.boolean)

#define TO_LOGIC(value) \
    ((!IS_NONE(value) && !IS_BOOLEAN(value)) || TO_BOOLEAN(value))

#endif // COG_VALUE_H
