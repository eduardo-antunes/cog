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

#include "lexer.h"
#include "common.h"
#include "parser.h"
#include "debug.h"
#include "box.h"
#include "vm.h"

int main()
{
    char expr[1024];
    fgets(expr, 1024, stdin);

    Parser pr;
    parser_init(&pr, expr);
    Box box;
    box_init(&box);
    bool alright = parse(&pr, &box);

    if(alright) disassemble(&box);
    box_free(&box);

    return 0;
}
