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
#include <stdarg.h>

#include "array.h"
#include "box.h"
#include "value.h"
#include "vm.h"
#include "opcodes.h"

// Types of operations

#define add(a, b) ((a) + (b))
#define sub(a, b) ((a) - (b))
#define mul(a, b) ((a) * (b))
#define div(a, b) ((a) / (b))

#define less(a, b) ((a) < (b))
#define greater(a, b) ((a) > (b))

#define BIN_NUMERIC_OP(type_value, op) {       \
    Cog_value b = pop();                       \
    Cog_value a = pop();                       \
    if(!IS_NUMBER(a) || !IS_NUMBER(b))         \
        return RES_ERROR;                      \
                                               \
    double x = TO_NUMBER(a), y = TO_NUMBER(b); \
    push(type_value(op(x, y)));                \
}

#define and(p, q) ((p) && (q))
#define or(p, q) ((p) || (q))

#define BIN_LOGIC_OP(op) {                 \
    Cog_value b = pop();                   \
    Cog_value a = pop();                   \
    bool p = TO_LOGIC(a), q = TO_LOGIC(b); \
    push(BOOLEAN_VALUE(op(p, q)));         \
}

// Public interface

void cog_vm_init(Cog_vm *vm) {
    vm->ip = NULL;
    cog_array_init(&vm->stack, 256);
}

void cog_vm_free(Cog_vm *vm) {
    vm->ip = NULL;
    cog_array_free(&vm->stack);
}

Cog_vm_result execute(Cog_vm *vm, const Box *box) {

    #define push(value) cog_array_push(&vm->stack, (value))
    #define pop() cog_array_pop(&vm->stack)
    #define end() &box->code[box->count]

    uint8_t addr;
    vm->ip = box->code;
    while(vm->ip != end()) {
        switch(*vm->ip) {
            case OP_NEG: {
                Cog_value a = pop();
                if(!IS_NUMBER(a)) return RES_ERROR;
                double x = TO_NUMBER(a);
                push(NUMBER_VALUE(-x));
                break;
            }
            case OP_ADD:
                BIN_NUMERIC_OP(NUMBER_VALUE, add);
                break;
            case OP_SUB:
                BIN_NUMERIC_OP(NUMBER_VALUE, sub);
                break;
            case OP_MUL:
                BIN_NUMERIC_OP(NUMBER_VALUE, mul);
                break;
            case OP_DIV:
                BIN_NUMERIC_OP(NUMBER_VALUE, div);
                break;

            case OP_NOT: {
                Cog_value a = pop();
                bool b = TO_LOGIC(a);
                push(BOOLEAN_VALUE(!b));
                break;
            }
            case OP_EQ: {
                Cog_value b = pop(), a = pop();
                bool p = cog_value_equal(a, b);
                push(BOOLEAN_VALUE(p));
                break;
            }
            case OP_LT:
                BIN_NUMERIC_OP(BOOLEAN_VALUE, less);
                break;
            case OP_GT:
                BIN_NUMERIC_OP(BOOLEAN_VALUE, greater);
                break;
            case OP_AND:
                BIN_LOGIC_OP(and);
                break;
            case OP_OR:
                BIN_LOGIC_OP(or);
                break;

            case OP_PSH:
                addr = *(++vm->ip);
                Cog_value a = cog_array_get(&box->constants, addr);
                push(a);
                break;
            case OP_PSH_TRUE:
                push(BOOLEAN_VALUE(true));
                break;
            case OP_PSH_FALSE:
                push(BOOLEAN_VALUE(false));
                break;
            case OP_PSH_NONE:
                push(NONE_VALUE);
                break;

            case OP_RET: {
                // Print the final result and exit (temporary)
                Cog_value expr = pop();
                printf("=> ");
                cog_value_print(expr);
                printf("\n");
                return RES_OK;
            }

            default:
                eprintf("Unimplemented operation\n");
                return RES_ERROR;
        }
        ++vm->ip;
    }
    return RES_OK;

    #undef push
    #undef pop
    #undef end
}

// Cleaning up local macros

#undef add
#undef sub
#undef mul
#undef div
#undef less
#undef greater
#undef and
#undef or

#undef BIN_NUMERIC_OP
#undef BIN_LOGIC_OP
