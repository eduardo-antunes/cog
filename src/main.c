#include <stdio.h>

#include "lexer.h"

// Yet another attempt to build an interpreter

int main()
{
    char expr[1024];
    fgets(expr, 1024, stdin);

    Lexer lex;
    init_lexer(&lex, expr);
    Token tok;
    while(1) {
        tok = get_token(&lex);
        if(tok.type == TOKEN_END) break;
        printf("%d %.*s\n", tok.type, tok.offset, tok.start);
    }
    return 0;
}
