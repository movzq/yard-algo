#ifndef _YARD_ALGO_H
#define _YARD_ALGO_H
#include <stdio.h>

enum yardTokenType {
    YARD_TOKEN_TYPE_SUB = '-',
    YARD_TOKEN_TYPE_ADD = '+',
    YARD_TOKEN_TYPE_MUL = '*',
    YARD_TOKEN_TYPE_DIV = '/',
    YARD_TOKEN_TYPE_LPA = '(',
    YARD_TOKEN_TYPE_RPA = ')',
    YARD_TOKEN_TYPE_NUM = '0'
};

struct yardToken {
    char* data;
    enum yardTokenType type;
};

struct yardAlgo {
    struct yardToken* expression;
    enum yardTokenType* symbols;
    size_t sizexpr;
    size_t numsymbols;
};

struct yardAlgo* yard_make ();
void yard_pushToken (struct yardAlgo*, const char*);
double yard_perform (struct yardAlgo*);

#endif
