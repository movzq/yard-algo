#include "yard-algo.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

bool _isOperator (const char op)
{
    return op == '+' || op == '-' || op == '*' || op == '/';
}

enum yardTokenType _isNumOrPar (const char*);
void _pushIntoQueue (struct yardAlgo*, const char*, const enum yardTokenType);
bool _samePrecedence (struct yardAlgo*, const enum yardTokenType);

struct yardAlgo* yard_make ()
{
    struct yardAlgo* yarda = (struct yardAlgo*) malloc(sizeof(struct yardAlgo));
    yarda->queue = (struct yardToken*) malloc(0);
    yarda->stack = (enum yardTokenType*) malloc(0);
    yarda->sizequeue = 0;
    yarda->sizestack = 0;
    return yarda;
}

/* Returns true if the token is where it gotta be, and false
 * if the order of the operation is given in a wrong way. Remember
 * this program works for tokens already parsed, however the program
 * will be able to analize if the expression was written in a right way. */
bool yard_evaluateToken (struct yardAlgo* yarda, const char* token)
{
    assert(yarda);
    assert(token);

    /* Number -> Operator.
     * Operator -> Number/Parentheses.
     * Left parentheses -> Number.
     * Right parentheses -> Operator. */
    static bool mustbe_opr = false;

    if (!mustbe_opr) {
        enum yardTokenType twas = _isNumOrPar(token);
        if (twas == YARD_TOKEN_TYPE_WTF)
            return false;

        if (twas == YARD_TOKEN_TYPE_NUM) {
            _pushIntoQueue(yarda, token, YARD_TOKEN_TYPE_NUM);
            mustbe_opr = true;
        }
    }
    else {
        if (!_isOperator(token[0]))
            return false;

        mustbe_opr = false;
        if (!_samePrecedence(yarda, token[0])) {
            yarda->stack = (enum yardTokenType*) realloc(
                yarda->stack,
                ++yarda->sizestack * sizeof(enum yardTokenType)
            );
            yarda->stack[yarda->sizestack - 1] = token[0];
        }
    }

    return true;
}

void yard_solve (struct yardAlgo* yarda)
{
    assert(yarda);
    for (size_t i = yarda->sizestack - 1; i != -1; i--)
        _pushIntoQueue(yarda, (char*) &yarda->stack[i], yarda->stack[i]);
    free(yarda->stack);

    for (size_t i = 0; i < yarda->sizequeue; i++)
        printf("%s\n", yarda->queue[i].data);
}

enum yardTokenType _isNumOrPar (const char* token)
{
    const char fchar = token[0];
    if (fchar == '(')
        return YARD_TOKEN_TYPE_LPA;

    if (fchar == ')')
        return YARD_TOKEN_TYPE_RPA;

    if (isdigit(fchar))
        return YARD_TOKEN_TYPE_NUM;

    if (strlen(token) >= 2 && isdigit(token[1]))
        return YARD_TOKEN_TYPE_NUM;

    return YARD_TOKEN_TYPE_WTF;
}

void _pushIntoQueue (struct yardAlgo* yarda, const char* data, const enum yardTokenType type)
{
    yarda->queue = (struct yardToken*) realloc(
        yarda->queue,
        ++yarda->sizequeue * sizeof(struct yardToken)
    );

    struct yardToken newt = {
        .data = (char*) malloc(strlen(data) + 1),
        .type = type
    };

    strcpy(newt.data, data);
    yarda->queue[yarda->sizequeue - 1] = newt;
}

bool _samePrecedence (struct yardAlgo* yarda, const enum yardTokenType curr)
{
    if (!yarda->sizestack)
        return false;

    enum yardTokenType prev = yarda->stack[yarda->sizestack - 1];
    bool isit = ((curr == '-' || curr == '+') && (prev == '+' || prev == '-')) ||
                ((curr == '*' || curr == '*') && (prev == '*' || prev == '/'));

    if (isit) {
        _pushIntoQueue(yarda, (char*) &prev, prev);
        yarda->stack[yarda->sizestack - 1] = curr;
    }
    return isit;
}

int main ()
{
    char* expr[] = {
        "4", "+", "1", "-", "5", "+", "19", "-", "4", "+", "2"
    };

    struct yardAlgo* yalgo = yard_make();
    for (size_t i = 0; i < 11; i++)
        yard_evaluateToken(yalgo, expr[i]);
    yard_solve(yalgo);
    return 0;
}
