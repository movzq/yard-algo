#ifndef _YARD_ALGO_H
#define _YARD_ALGO_H
#include <stdio.h>
#include <stdbool.h>

enum yardTokenType {
    YARD_TOKEN_TYPE_SUB = '-',
    YARD_TOKEN_TYPE_ADD = '+',
    YARD_TOKEN_TYPE_DIV = '/',
    YARD_TOKEN_TYPE_MUL = '*',
    YARD_TOKEN_TYPE_LPA = '(',
    YARD_TOKEN_TYPE_RPA = ')',
    YARD_TOKEN_TYPE_NUM = '0',
    YARD_TOKEN_TYPE_WTF = '?'
};

struct yardToken {
    char* data;
    enum yardTokenType type;
};

struct yardAlgo {
    struct yardToken* queue;
    enum yardTokenType* stack;

    size_t sizequeue;
    size_t sizestack;
};

struct yardAlgo* yard_make ();
bool yard_evaluateToken (struct yardAlgo*, const char*);
void yard_solve (struct yardAlgo*);

#endif
