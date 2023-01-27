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

// Yet another attempt to build a programming language

#include <stdio.h>

#include "box.h"
#include "common.h"
#include "compiler.h"
#include "debug.h"
#include "value.h"
#include "vm.h"

int main(void) {
    char expr[1024];
    Cog_vm vm;
    cog_vm_init(&vm);
    while(fgets(expr, 1024, stdin) != NULL) {
        Box box;
        box_init(&box);
        bool alright = compile(expr, &box);
        if(alright) {
            Cog_vm_result res = execute(&vm, &box);
            if(res == RES_ERROR)
                eprintf("(!) Runtime error ocurred!\n");
        }
        box_free(&box);
    }
    cog_vm_free(&vm);
    return 0;
}
