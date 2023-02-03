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
#include <stdlib.h>

#include "common.h"
#include "memory.h"

void *cog_realloc(void *ptr, size_t old_size, size_t new_size) {
    if(new_size == 0) {
        free(ptr);
        return NULL;
    }
    void *new_ptr = NULL;
    new_ptr = realloc(ptr, new_size);
    if(new_ptr == NULL) {
        fprintf(stderr, "Could not allocate memory\n");
        exit(1);
    }
    return new_ptr;
}
