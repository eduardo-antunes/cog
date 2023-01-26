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
#include "value.h"
#include "vm.h"

// Types of operations:

#define add(a, b)     ((a) + (b))
#define sub(a, b)     ((a) - (b))
#define mul(a, b)     ((a) * (b))
#define div(a, b)     ((a) / (b))
#define less(a, b)    ((a) < (b))
#define greater(a, b) ((a) > (b))

#define BIN_ARITH_OP(result_type, op) {                    \
        Cog_value v2 = cog_array_pop(&vm->stack);          \
        Cog_value v1 = cog_array_pop(&vm->stack);          \
                                                           \
        if(!IS_NUM(v1) || !IS_NUM(v2))                     \
            return RES_ERR_TYPE;                           \
                                                           \
        double a = TO_NUM(v1), b = TO_NUM(v2);             \
        cog_array_push(&vm->stack, result_type(op(a, b))); \
    }

#define and(p, q) ((p) && (q))
#define or(p, q)  ((p) || (q))

#define BIN_LOGIC_OP(result_type, op) {                    \
        Cog_value v2 = cog_array_pop(&vm->stack);          \
        Cog_value v1 = cog_array_pop(&vm->stack);          \
                                                           \
        double p = LOGIC_VAL(v1), q = LOGIC_VAL(v2);       \
        cog_array_push(&vm->stack, result_type(op(p, q))); \
    }

static bool cog_value_equal(Cog_value v1, Cog_value v2) {
    // values are only equal if their type is the same
    if(v1.type != v2.type) return false;
    switch(v1.type) {
        case TYPE_NUMBER:
            return TO_NUM(v1) == TO_NUM(v2);
        case TYPE_BOOLEAN:
            return TO_BOOL(v1) == TO_BOOL(v2);
        case TYPE_NONE:
            return true;
    }
    return false; // unreachable
}

// Public interface

void cog_vm_init(Cog_vm *vm) {
    cog_array_init(&vm->stack);
}

void cog_vm_free(Cog_vm *vm) {
    cog_array_free(&vm->stack);
}

Exec_result cog_vm_execute(Cog_vm *vm, Box *box) {

#define push(v) cog_array_push(&vm->stack, (v))
#define pop()   cog_array_pop(&vm->stack)

    uint8_t addr;
    for(unsigned i = 0; i < box->count; ++i) {
        switch(box->code[i]) {
            // Arithmetic ops
            case OP_NEG: {
                Cog_value v = pop();
                if(!IS_NUM(v)) return RES_ERR_TYPE;
                double x = TO_NUM(v);
                push(COG_NUM(-x));
                break;
            }
            case OP_ADD:
                BIN_ARITH_OP(COG_NUM, add);
                break;
            case OP_SUB:
                BIN_ARITH_OP(COG_NUM, sub);
                break;
            case OP_MUL:
                BIN_ARITH_OP(COG_NUM, mul);
                break;
            case OP_DIV:
                BIN_ARITH_OP(COG_NUM, div);
                break;

            // Relational ops
            case OP_EQ: {
                Cog_value b = pop(), a = pop();
                bool equal = cog_value_equal(a, b);
                push(COG_BOOL(equal));
                break;
            }
            case OP_LT:
                BIN_ARITH_OP(COG_BOOL, less);
                break;
            case OP_GT:
                BIN_ARITH_OP(COG_BOOL, greater);
                break;

            // Boolean ops
            case OP_NOT: {
                Cog_value b = pop();
                b = COG_BOOL(!LOGIC_VAL(b));
                push(b);
                break;
            }
            case OP_AND:
                BIN_LOGIC_OP(COG_BOOL, and); // TODO short-circuit
                break;
            case OP_OR:
                BIN_LOGIC_OP(COG_BOOL, or); // TODO short-circuit
                break;

            // Stack ops
            case OP_PSH:
                addr = box->code[++i];
                Cog_value v = cog_array_get(&box->constants, addr);
                push(v);
                break;
            case OP_PSH_TRUE:
                push(COG_BOOL(true));
                break;
            case OP_PSH_FALSE:
                push(COG_BOOL(false));
                break;
            case OP_PSH_NONE:
                push(COG_NONE);
                break;
        }
    }
    return RES_OK;

#undef push
#undef pop
}

#undef BIN_ARITH_OP
#undef BIN_LOGIC_OP
#undef add
#undef sub
#undef mul
#undef div
#undef less
#undef greater
#undef and
#undef or
