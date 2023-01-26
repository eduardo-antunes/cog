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

#ifndef COG_LEXER_H
#define COG_LEXER_H

typedef struct {
    const char *start;
    const char *current;
    int line;
} Lexer;

typedef enum {
    // Keywords
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_NOT,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_NONE,

    // Operators
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_OPEN_PAREN,
    TOKEN_CLOSE_PAREN,

    TOKEN_LESS,
    TOKEN_LESS_EQUAL,
    TOKEN_GREATER,
    TOKEN_GREATER_EQUAL,
    TOKEN_EQUAL,
    TOKEN_EQUAL_EQUAL,
    TOKEN_NOT_EQUAL,

    // Values
    TOKEN_NUM,
    TOKEN_ID,

    // Special
    TOKEN_ERR,
    TOKEN_END,
} Token_t;

typedef struct {
    Token_t type;
    const char *start;
    int offset;
    int line;
} Token;

void lexer_init(Lexer *lex, const char *source);

Token lexer_get_tok(Lexer *lex);

#endif // COG_LEXER_H
