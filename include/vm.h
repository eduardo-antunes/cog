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

typedef struct {
    uint8_t *ip;
    Cog_array stack;
} Cog_env;

typedef enum {
    RES_OK,
    RES_ERROR,
} Cog_result;

void cog_env_init(Cog_env *env);

Cog_result execute(Cog_env *env, const Box *box);

void cog_env_free(Cog_env *env);

#endif // COG_VM_H
