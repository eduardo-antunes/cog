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
    Cog_type type;
    union {
        double number;
        bool boolean;
    } as;
} Cog_value;

void cog_value_print(Cog_value value);

bool cog_values_equal(Cog_value a, Cog_value b);

// type checks

#define IS_NUMBER(value) ((value).type == TYPE_NUMBER)

#define IS_BOOLEAN(value) ((value).type == TYPE_BOOLEAN)

#define IS_NONE(value) ((value).type == TYPE_NONE)

// conversion: C value -> Cog value

#define COG_NUMBER(n) ((Cog_value) { TYPE_NUMBER, .as.number = (n) })

#define COG_BOOLEAN(b) ((Cog_value) { TYPE_BOOLEAN, .as.boolean = (b) })

#define COG_NONE ((Cog_value) { TYPE_NONE, .as.number = 0 })

// conversion: Cog value -> C value

#define TO_DOUBLE(value) ((value).as.number)

#define TO_BOOL(value) ((value).as.boolean)

#define IS_TRUTHY(value) \
    ((!IS_NONE(value) && !IS_BOOLEAN(value)) || TO_BOOL(value))

#endif // COG_VALUE_H
