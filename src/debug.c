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

#include "box.h"
#include "common.h"
#include "debug.h"
#include "opcodes.h"
#include "value.h"

void disassemble(const Box *box) {
    uint8_t addr;
    for(unsigned ip = 0; ip < box->count; ++ip) {
        switch(box->code[ip]) {
            case OP_NEG:
                printf("neg\n");
                break;
            case OP_ADD:
                printf("add\n");
                break;
            case OP_SUB:
                printf("sub\n");
                break;
            case OP_MUL:
                printf("mul\n");
                break;
            case OP_DIV:
                printf("div\n");
                break;
            case OP_NOT:
                printf("not\n");
                break;
            case OP_EQ:
                printf("eq\n");
                break;
            case OP_LT:
                printf("lt\n");
                break;
            case OP_GT:
                printf("gt\n");
                break;
            case OP_AND:
                printf("and\n");
                break;
            case OP_OR:
                printf("or\n");
                break;
            case OP_PSH:
                addr = box->code[++ip];
                Cog_value value = cog_array_get(&box->constants, addr);
                printf("psh ");
                cog_value_print(value);
                printf("\n");
                break;
            case OP_PSH_TRUE:
                printf("psh true\n");
                break;
            case OP_PSH_FALSE:
                printf("psh false\n");
                break;
            case OP_PSH_NONE:
                printf("psh none\n");
                break;
            case OP_RET:
                printf("ret\n");
                break;
            default:
                printf("???\n");
        }
    }
}
