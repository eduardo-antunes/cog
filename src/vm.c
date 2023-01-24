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

// Arithmetic ops

static Result vm_neg(Cog_vm *vm) {
    Cog_value v = cog_array_pop(&vm->stack);
    if(!IS_NUM(v)) 
        return R_ERR_TYPE;

    double x = TO_NUM(v);
    cog_array_push(&vm->stack, COG_NUM(-x));
    return R_OK;
}

static Result vm_add(Cog_vm *vm) {
    Cog_value v2 = cog_array_pop(&vm->stack);
    Cog_value v1 = cog_array_pop(&vm->stack);
    if(!IS_NUM(v1) || !IS_NUM(v2)) 
        return R_ERR_TYPE;

    double x = TO_NUM(v1), y = TO_NUM(v2);
    cog_array_push(&vm->stack, COG_NUM(x + y));
    return R_OK;
}

static Result vm_sub(Cog_vm *vm) {
    Cog_value v2 = cog_array_pop(&vm->stack);
    Cog_value v1 = cog_array_pop(&vm->stack);
    if(!IS_NUM(v1) || !IS_NUM(v2)) 
        return R_ERR_TYPE;

    double x = TO_NUM(v1), y = TO_NUM(v2);
    cog_array_push(&vm->stack, COG_NUM(x - y));
    return R_OK;
}

static Result vm_mul(Cog_vm *vm) {
    Cog_value v2 = cog_array_pop(&vm->stack);
    Cog_value v1 = cog_array_pop(&vm->stack);
    if(!IS_NUM(v1) || !IS_NUM(v2)) 
        return R_ERR_TYPE;

    double x = TO_NUM(v1), y = TO_NUM(v2);
    cog_array_push(&vm->stack, COG_NUM(x * y));
    return R_OK;
}

static Result vm_div(Cog_vm *vm) {
    Cog_value v2 = cog_array_pop(&vm->stack);
    Cog_value v1 = cog_array_pop(&vm->stack);
    if(!IS_NUM(v1) || !IS_NUM(v2)) 
        return R_ERR_TYPE;

    double x = TO_NUM(v1), y = TO_NUM(v2);
    cog_array_push(&vm->stack, COG_NUM(x / y));
    return R_OK;
}

// Logic ops

static Result vm_not(Cog_vm *vm) {
    Cog_value v = cog_array_pop(&vm->stack);
    v = COG_BOOL(!LOGIC_VAL(v));
    cog_array_push(&vm->stack, v);
    return R_OK;
}

static Result vm_and(Cog_vm *vm) {
    Cog_value v2 = cog_array_pop(&vm->stack);
    Cog_value v1 = cog_array_pop(&vm->stack);
    bool b = LOGIC_VAL(v1) && LOGIC_VAL(v2);
    cog_array_push(&vm->stack, COG_BOOL(b));
    return R_OK;
}

static Result vm_or(Cog_vm *vm) {
    Cog_value v2 = cog_array_pop(&vm->stack);
    Cog_value v1 = cog_array_pop(&vm->stack);
    bool b = LOGIC_VAL(v1) || LOGIC_VAL(v2);
    cog_array_push(&vm->stack, COG_BOOL(b));
    return R_OK;
}

// Public interface

void vm_start(Cog_vm *vm) {
    cog_array_init(&vm->stack);
}

void vm_end(Cog_vm *vm) {
    cog_array_free(&vm->stack);
}

Cog_value vm_execute(Cog_vm *vm, Box *box) {
    uint8_t addr;
    Result res = R_OK;
    for(unsigned i = 0; i < box->count; ++i) {
        switch(box->code[i]) {
            // Arithmetic ops
            case OP_NEG:
                res = vm_neg(vm);
                break;
            case OP_ADD:
                res = vm_add(vm);
                break;
            case OP_SUB:
                res = vm_sub(vm);
                break;
            case OP_MUL:
                res = vm_mul(vm);
                break;
            case OP_DIV:
                res = vm_div(vm);
                break;

            // Boolean ops
            case OP_NOT:
                res = vm_not(vm);
                break;
            case OP_AND:
                res = vm_and(vm);
                break;
            case OP_OR:
                res = vm_or(vm);
                break;

            // Stack ops
            case OP_PSH:
                addr = box->code[++i];
                Cog_value v = cog_array_get(&box->constants, addr);
                cog_array_push(&vm->stack, v);
                break;
            case OP_PSH_TRUE:
                cog_array_push(&vm->stack, COG_BOOL(true));
                break;
            case OP_PSH_FALSE:
                cog_array_push(&vm->stack, COG_BOOL(false));
                break;
            case OP_PSH_NULL:
                cog_array_push(&vm->stack, COG_NULL());
                break;
        }
    }
    if(res != R_OK) eprintf("Runtime error ocurred!\n");
    return cog_array_pop(&vm->stack);
}
