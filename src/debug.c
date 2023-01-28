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

static int disassemble_inst(const Box *box, uint8_t *ptr) {
    uint8_t addr;
    switch(*ptr) {
        case OP_NEG:
            printf("neg\n");
            return 1;
        case OP_ADD:
            printf("add\n");
            return 1;
        case OP_SUB:
            printf("sub\n");
            return 1;
        case OP_MUL:
            printf("mul\n");
            return 1;
        case OP_DIV:
            printf("div\n");
            return 1;
        case OP_NOT:
            printf("not\n");
            return 1;
        case OP_EQ:
            printf("eq\n");
            return 1;
        case OP_LT:
            printf("lt\n");
            return 1;
        case OP_GT:
            printf("gt\n");
            return 1;
        case OP_AND:
            printf("and\n");
            return 1;
        case OP_OR:
            printf("or\n");
            return 1;
        case OP_PSH:
            addr = *(++ptr);
            Cog_value value = cog_array_get(&box->constants, addr);
            printf("psh ");
            cog_value_print(value);
            printf("\n");
            return 2;
        case OP_PSH_TRUE:
            printf("psh true\n");
            return 1;
        case OP_PSH_FALSE:
            printf("psh false\n");
            return 1;
        case OP_PSH_NONE:
            printf("psh none\n");
            return 1;
        case OP_RET:
            printf("ret\n");
            return 1;
        default:
            printf("???\n");
            return 1;
    }

}

void disassemble(const Box *box) {
    uint8_t *ptr = box->code;
    while(ptr != &box->code[box->count])
        ptr += disassemble_inst(box, ptr);
}
