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
    double x = TO_DOUBLE(a), y = TO_DOUBLE(b); \
    push(type_value(op(x, y)));                \
}

#define and(p, q) ((p) && (q))
#define or(p, q) ((p) || (q))

#define BIN_LOGIC_OP(op) {                   \
    Cog_value b = pop();                     \
    Cog_value a = pop();                     \
    bool p = IS_TRUTHY(a), q = IS_TRUTHY(b); \
    push(COG_BOOLEAN(op(p, q)));             \
}

// Public interface

void cog_env_init(Cog_env *env) {
    env->ip = NULL;
    cog_array_init(&env->stack, 256);
}

void cog_env_free(Cog_env *env) {
    env->ip = NULL;
    cog_array_free(&env->stack);
}

Cog_result execute(Cog_env *env, const Box *box) {

    #define push(value) cog_array_push(&env->stack, (value))
    #define pop() cog_array_pop(&env->stack)
    #define end() &box->code[box->count]

    uint8_t addr;
    env->ip = box->code;
    while(env->ip != end()) {
        switch(*env->ip) {
            case OP_NEG: {
                Cog_value a = pop();
                if(!IS_NUMBER(a)) return RES_ERROR;
                double x = TO_DOUBLE(a);
                push(COG_NUMBER(-x));
                break;
            }
            case OP_ADD:
                BIN_NUMERIC_OP(COG_NUMBER, add);
                break;
            case OP_SUB:
                BIN_NUMERIC_OP(COG_NUMBER, sub);
                break;
            case OP_MUL:
                BIN_NUMERIC_OP(COG_NUMBER, mul);
                break;
            case OP_DIV:
                BIN_NUMERIC_OP(COG_NUMBER, div);
                break;

            case OP_NOT: {
                Cog_value a = pop();
                bool b = IS_TRUTHY(a);
                push(COG_BOOLEAN(!b));
                break;
            }
            case OP_EQ: {
                Cog_value b = pop(), a = pop();
                bool p = cog_values_equal(a, b);
                push(COG_BOOLEAN(p));
                break;
            }
            case OP_LT:
                BIN_NUMERIC_OP(COG_BOOLEAN, less);
                break;
            case OP_GT:
                BIN_NUMERIC_OP(COG_BOOLEAN, greater);
                break;
            case OP_AND:
                BIN_LOGIC_OP(and);
                break;
            case OP_OR:
                BIN_LOGIC_OP(or);
                break;

            case OP_PSH:
                addr = *(++env->ip);
                Cog_value a = cog_array_get(&box->constants, addr);
                push(a);
                break;
            case OP_PSH_TRUE:
                push(COG_BOOLEAN(true));
                break;
            case OP_PSH_FALSE:
                push(COG_BOOLEAN(false));
                break;
            case OP_PSH_NONE:
                push(COG_NONE);
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
        ++env->ip;
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
