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

#include <stdio.h>
#include "common.h"
#include "value.h"

void cog_value_print(Cog_value value) {
    switch(value.type) {
        case TYPE_NUMBER:
            printf("%g", TO_DOUBLE(value));
            break;
        case TYPE_BOOLEAN:
            printf(TO_BOOL(value) ? "true" : "false");
            break;
        case TYPE_NONE:
            printf("none");
            break;
        default:
            // should be unreachable
            eprintf("???");
    }
}

bool cog_values_equal(Cog_value a, Cog_value b) {
    if(a.type != b.type) 
        // Values of different types can never be equal
        return false;

    // Each type has a different way to check for equality
    switch(a.type) {
        case TYPE_NUMBER:
            return TO_DOUBLE(a) == TO_DOUBLE(b);
        case TYPE_BOOLEAN:
            return TO_BOOL(a) == TO_BOOL(b);
        case TYPE_NONE:
            return true;
        default:
            // should be unreachable
            eprintf("(?) Equality not implemented for a type\n");
            return false;
    }
}
