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
#include "value.h"
#include "vm.h"
#include "opcodes.h"

// Core functions

static int push(Cog_vm *vm, Cog_value value) {
    if(vm->stack_top + 1 > COG_VM_STACK_MAX) {
        // TODO improve error handling
        eprintf("(!) Stack Overflow in the VM\n");
        return 1;
    }
    vm->stack[vm->stack_top++] = value;
    return 0;
}

static Cog_value pop(Cog_vm *vm) {
    if(vm->stack_top == 0) {
        // TODO improve error handling
        eprintf("(!) Stack Underflow in the VM\n");
        return NONE_VALUE;
    }
    int i = --vm->stack_top;
    return vm->stack[i];
}

// Types of operations

#define add(a, b) ((a) + (b))
#define sub(a, b) ((a) - (b))
#define mul(a, b) ((a) * (b))
#define div(a, b) ((a) / (b))

#define less(a, b) ((a) < (b))
#define greater(a, b) ((a) > (b))

#define BIN_NUMERIC_OP(vm, type_value, op) {   \
    Cog_value b = pop(vm);                     \
    Cog_value a = pop(vm);                     \
    if(!IS_NUMBER(a) || !IS_NUMBER(b))         \
        return RES_ERROR;                      \
                                               \
    double x = TO_NUMBER(a), y = TO_NUMBER(b); \
    push(vm, type_value(op(x, y)));            \
}

#define and(p, q) ((p) && (q))
#define or(p, q) ((p) || (q))

#define BIN_LOGIC_OP(vm, op) {             \
    Cog_value b = pop(vm);                 \
    Cog_value a = pop(vm);                 \
    bool p = TO_LOGIC(a), q = TO_LOGIC(b); \
    push(vm, BOOLEAN_VALUE(op(p, q)));     \
}


// Public interface

void cog_vm_init(Cog_vm *vm) {
    vm->stack_top = 0;
}

void cog_vm_free(Cog_vm *vm) {
    vm->stack_top = 0;
}

Cog_vm_result execute(Cog_vm *vm, const Box *box) {
    uint8_t addr;
    for(unsigned ip = 0; ip < box->count; ++ip) {
        switch(box->code[ip]) {
            case OP_NEG: {
                Cog_value a = pop(vm);
                if(!IS_NUMBER(a)) return RES_ERROR;
                double x = TO_NUMBER(a);
                push(vm, NUMBER_VALUE(-x));
                break;
            }
            case OP_ADD:
                BIN_NUMERIC_OP(vm, NUMBER_VALUE, add);
                break;
            case OP_SUB:
                BIN_NUMERIC_OP(vm, NUMBER_VALUE, sub);
                break;
            case OP_MUL:
                BIN_NUMERIC_OP(vm, NUMBER_VALUE, mul);
                break;
            case OP_DIV:
                BIN_NUMERIC_OP(vm, NUMBER_VALUE, div);
                break;

            case OP_NOT: {
                Cog_value a = pop(vm);
                bool b = TO_LOGIC(a);
                push(vm, BOOLEAN_VALUE(!b));
                break;
            }
            case OP_EQ: {
                Cog_value b = pop(vm), a = pop(vm);
                bool p = cog_value_equal(a, b);
                push(vm, BOOLEAN_VALUE(p));
                break;
            }
            case OP_LT:
                BIN_NUMERIC_OP(vm, BOOLEAN_VALUE, less);
                break;
            case OP_GT:
                BIN_NUMERIC_OP(vm, BOOLEAN_VALUE, greater);
                break;
            case OP_AND:
                BIN_LOGIC_OP(vm, and);
                break;
            case OP_OR:
                BIN_LOGIC_OP(vm, or);
                break;

            case OP_PSH:
                addr = box->code[++ip];
                Cog_value a = cog_array_get(&box->constants, addr);
                push(vm, a);
                break;
            case OP_PSH_TRUE:
                push(vm, BOOLEAN_VALUE(true));
                break;
            case OP_PSH_FALSE:
                push(vm, BOOLEAN_VALUE(false));
                break;
            case OP_PSH_NONE:
                push(vm, NONE_VALUE);
                break;

            case OP_RET: {
                // Print the final result and exit (temporary)
                Cog_value expr = pop(vm);
                printf("=> ");
                cog_value_print(expr);
                printf("\n");
                return RES_OK;
            }

            default:
                eprintf("Unimplemented operation\n");
                return RES_ERROR;
        }
    }
    return RES_OK;
}

#undef add
#undef sub
#undef mul
#undef div
#undef less
#undef greater
#undef BIN_NUMERIC_OP

#undef and
#undef or
#undef BIN_LOGIC_OP
