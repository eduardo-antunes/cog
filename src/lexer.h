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
