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

// Runtime operations

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

// Public interface

void vm_start(Cog_vm *vm) {
    cog_array_init(&vm->stack);
}

void vm_end(Cog_vm *vm) {
    cog_array_free(&vm->stack);
}

Cog_value vm_execute(Cog_vm *vm, Box *box) {
    uint8_t addr;
    cog_array_init(&vm->stack);
    for(unsigned i = 0; i < box->count; ++i) {
        switch(box->code[i]) {
            case OP_NEG:
                vm_neg(vm);
                break;
            case OP_ADD:
                vm_add(vm);
                break;
            case OP_SUB:
                vm_sub(vm);
                break;
            case OP_MUL:
                vm_mul(vm);
                break;
            case OP_DIV:
                vm_div(vm);
                break;

            // Loading operations
            case OP_PUSH:
                addr = box->code[++i];
                Cog_value v = cog_array_get(&box->constants, addr);
                cog_array_push(&vm->stack, v);
                break;

            // Others
            case OP_RETURN:
                break;
        }
    }
    return cog_array_pop(&vm->stack);
}
