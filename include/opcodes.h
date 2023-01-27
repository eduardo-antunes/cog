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

#ifndef COG_OPCODES_H
#define COG_OPCODES_H

typedef enum {
    // arithmetic
    OP_NEG,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,

    // logic and relational
    OP_NOT,
    OP_EQ,
    OP_LT,
    OP_GT,
    OP_AND,
    OP_OR,

    // stack manipulation
    OP_PSH,
    OP_PSH_TRUE,
    OP_PSH_FALSE,
    OP_PSH_NONE,

    // others
    OP_RET,
} Op_code;

#endif // COG_OPCODES_H
