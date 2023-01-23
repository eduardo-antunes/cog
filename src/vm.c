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

#include "box.h"
#include "common.h"
#include "value.h"
#include "vm.h"

Cog_val execute(Box *box) {
    uint8_t ind;
    Cog_val x, y;
    Cog_vector vec;
    vector_init(&vec);
    for(unsigned i = 0; i < box->count; ++i) {
        switch(box->code[i]) {
            // Unary operations
            case OP_NEG:
                x = vector_pop(&vec);
                vector_push(&vec, -x);
                break;

            // Binary operations
            case OP_ADD:
                y = vector_pop(&vec);
                x = vector_pop(&vec);
                vector_push(&vec, x + y);
                break;
            case OP_SUB:
                y = vector_pop(&vec);
                x = vector_pop(&vec);
                vector_push(&vec, x - y);
                break;
            case OP_MUL:
                y = vector_pop(&vec);
                x = vector_pop(&vec);
                vector_push(&vec, x * y);
                break;
            case OP_DIV:
                y = vector_pop(&vec);
                x = vector_pop(&vec);
                vector_push(&vec, x / y);
                break;

            // Constant loading operations
            case OP_PUSH:
                ind = box->code[++i];
                Cog_val val = vector_get(&box->constants, ind);
                vector_push(&vec, val);
                break;
        }
    }
    return vector_pop(&vec);
}
