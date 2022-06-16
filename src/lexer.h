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

// Lexing analysis component of cog

#ifndef COG_LEXER_H
#define COG_LEXER_H

typedef struct {
    const char *start;
    const char *current;
    int line;
} Lexer;

typedef enum {
    // 1 character:
    TOKEN_ADD,
    TOKEN_SUB,
    TOKEN_MUL,
    TOKEN_DIV,

    // n characters:
    TOKEN_NUM,

    // Special:
    TOKEN_ERR,
    TOKEN_END,
} Token_t;

typedef struct {
    const char *start;
    Token_t type;
    int offset;
    int line;
} Token;

// Initialize the lexer
void init_lexer(Lexer *lex, const char *source);

// Yield a single token from source
Token get_token(Lexer *lex);

#endif // COG_LEXER_H
