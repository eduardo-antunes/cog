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

#ifndef COG_VM_H
#define COG_VM_H

#include "box.h"
#include "value.h"

typedef enum {
    R_OK,
    R_ERR_TYPE,
} Result;

typedef struct {
    Cog_array stack;
} Cog_vm;

void vm_start(Cog_vm *vm);

Cog_value vm_execute(Cog_vm *vm, Box *box);

void vm_end(Cog_vm *vm);

#endif // COG_VM_H
