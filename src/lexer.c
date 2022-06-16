#include <string.h>
#include <ctype.h>

#include "lexer.h"

#define END '\0'

// Initialize the lexer
void init_lexer(Lexer *lex, const char *source)
{
    lex->start = source;
    lex->current = source;
    lex->line = 1;
}

// See the current character and move the
// lexer a character forward
static char advance(Lexer *lex)
{
    lex->current++;
    return lex->current[-1];
}

// See the current character
static char peek(Lexer *lex)
{
    return lex->current[0];
}

// Make a token using the lexer's current state
static Token mktok(Lexer *lex, Token_t t)
{
    Token tok;
    tok.start = lex->start;
    tok.offset = lex->current - lex->start;
    tok.line = lex->line;
    tok.type = t;
    return tok;
}

// Report an error as an error token
static Token lex_error(Lexer *lex, const char *why)
{
    Token err;
    err.type = TOKEN_ERR;
    err.line = lex->line;
    err.start = why;
    err.offset = strlen(why);
    return err;
}

// Tokenize a number, integer or floating-point
static Token number_tok(Lexer *lex)
{
    // Integer part
    while(isdigit(peek(lex))) advance(lex);

    // Optional decimal part
    if(peek(lex) == '.') {
        advance(lex);
        while(isdigit(peek(lex)))
            advance(lex);
    }

    return mktok(lex, TOKEN_NUM);
}

// Ignore whitespace and comments
static void ignore_whitespace(Lexer *lex)
{
    char ch;
    while(isspace(peek(lex)) || peek(lex) == '#') {
        ch = advance(lex);
        switch(ch) {
            case '\n':
                lex->line++;
                break;
            case '\r':
                if(peek(lex) == '\n')
                    advance(lex);
                lex->line++;
                break;
            case '#':
                while(peek(lex) != '\n' && peek(lex) != '\r')
                    advance(lex);
                break;
        }
    }
}

// Yield a single token from source
Token get_token(Lexer *lex)
{
    ignore_whitespace(lex);
    lex->start = lex->current;

    char ch = advance(lex);

    switch(ch) {
        case '+': return mktok(lex, TOKEN_ADD);
        case '-': return mktok(lex, TOKEN_SUB);
        case '*': return mktok(lex, TOKEN_MUL);
        case '/': return mktok(lex, TOKEN_DIV);
        case END: return mktok(lex, TOKEN_END);
    }

    if(isdigit(ch)) return number_tok(lex);

    return lex_error(lex, "Unrecognized char");
}
