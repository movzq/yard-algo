#ifndef _YARD_ALGO_H
#define _YARD_ALGO_H
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

enum yardTokenType {
    YARD_TOKEN_TYPE_B_PARN = '(',
    YARD_TOKEN_TYPE_E_PARN = ')',
    YARD_TOKEN_TYPE_ADD_OP = '+',
    YARD_TOKEN_TYPE_SUB_OP = '~',
    YARD_TOKEN_TYPE_MUL_OP = '*',
    YARD_TOKEN_TYPE_DIV_OP = '/',
};

struct yardExpr {
    double* nums;
    enum yardTokenType *symbols;
    double value;
};

struct yardExpr* yardalgo_init ();
bool yardalgo_pushToken (struct yardExpr*, char*);
void yardalgo_perform (struct yardExpr*);

#endif
