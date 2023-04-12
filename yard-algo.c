#include "yard-algo.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

/*
 * ================================================
 * Inline functions.
 * ================================================
 * */
bool _isOperator (const char op)
{
    return op == '+' || op == '-' || op == '*' || op == '/';
}

bool _samePrecedence (const enum yardTokenType top, const enum yardTokenType new)
{
    bool low = (top == '+' || top == '-') && (new == '+' || new == '-');
    bool high = (top == '*' || top == '/') && (new == '*' || new == '/');
    return low || high;
}

bool _lowPrecedence (const enum yardTokenType top, const enum yardTokenType new)
{
    return (new == '-' || new == '+') && (top == '*' || top == '/');
}

/*
 * ================================================
 * Defintion of funcions used only in this file.
 * ================================================
 * */
enum yardTokenType _isNumOrPar (const char*);
void _pushIntoQueue (struct yardAlgo*, const char*, const enum yardTokenType);
void _precedenceIssue (struct yardAlgo*, enum yardTokenType);
void _doArithmetic (double*, size_t*, const enum yardTokenType);

/*
 * ================================================
 * Implementing functions of the header file.
 * ================================================
 * */
struct yardAlgo* yard_make ()
{
    struct yardAlgo* yarda = (struct yardAlgo*) malloc(sizeof(struct yardAlgo));
    yarda->queue = (struct yardToken*) malloc(0);
    yarda->stack = (enum yardTokenType*) malloc(0);
    yarda->sizequeue = 0;
    yarda->sizestack = 0;
    return yarda;
}

bool yard_evaluateToken (struct yardAlgo* yarda, const char* token)
{
    assert(yarda);
    assert(token);

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
        _precedenceIssue(yarda, token[0]);
        mustbe_opr = false;
    }

    /* Returns true if the token is where it gotta be, and false
     * if the order of the operation is given in a wrong way. Remember
     * this program works for tokens already parsed, however the program
     * will be able to analize if the expression was written in a right way. */
    return true;
}

void yard_solve (struct yardAlgo* yarda)
{
    assert(yarda);
    for (size_t i = yarda->sizestack - 1; i != -1; i--)
        _pushIntoQueue(yarda, (char*) &yarda->stack[i], yarda->stack[i]);
    free(yarda->stack);

    double* output = (double*) malloc(0);
    size_t sizeout = 0;

    for (size_t i = 0; i < yarda->sizequeue; i++) {
        enum yardTokenType type = yarda->queue[i].type;

        if (type != YARD_TOKEN_TYPE_NUM)
            _doArithmetic(output, &sizeout, type);
        else {
            output = (double*) realloc(output, ++sizeout * sizeof(double));
            output[sizeout - 1] = strtod(yarda->queue[i].data, NULL);
        }
        free(yarda->queue[i].data);
    }

    printf("%f\n", output[0]);
    free(yarda->queue);
    free(yarda);
    free(output);
}

/*
 * ================================================
 * Implementing functions of the C file.
 * ================================================
 * */
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

void _precedenceIssue (struct yardAlgo* yarda, enum yardTokenType incoming)
{
    yarda->stack = (enum yardTokenType*) realloc(yarda->stack, ++yarda->sizestack * sizeof(enum yardTokenType));
    yarda->stack[yarda->sizestack - 1] = incoming;
    if (yarda->sizestack == 1)
        return;

    enum yardTokenType previous = yarda->stack[yarda->sizestack - 2];
    while (_lowPrecedence(previous, incoming) || _samePrecedence(previous, incoming)) {
        _pushIntoQueue(yarda, (char*) &previous, previous);
        yarda->stack[yarda->sizestack - 2] = incoming;
        yarda->stack = (enum yardTokenType*) realloc(yarda->stack, --yarda->sizestack * sizeof(enum yardTokenType));

        incoming = yarda->stack[yarda->sizestack - 1];
        previous = yarda->stack[yarda->sizestack - 2];
    }
}

void _doArithmetic (double* output, size_t* outsize, const enum yardTokenType type)
{
    double n1 = output[*outsize - 2];
    double n2 = output[*outsize - 1];
    double value = 0;

    switch (type) {
        case YARD_TOKEN_TYPE_ADD: { value = n1 + n2; break; }
        case YARD_TOKEN_TYPE_SUB: { value = n1 - n2; break; }
        case YARD_TOKEN_TYPE_MUL: { value = n1 * n2; break; }
        case YARD_TOKEN_TYPE_DIV: { value = n1 / n2; break; }
    }

    *outsize -= 1;
    output = (double*) realloc(output, *outsize * sizeof(double));
    output[*outsize - 1] = value;
}

int main ()
{
    char* expr[] = {"3", "/", "0", "+", "1"};

    struct yardAlgo* yalgo = yard_make();
    for (size_t i = 0; i < 5; i++) {
        if (!yard_evaluateToken(yalgo, expr[i])) {
            puts("WRONG");
        }
    }
    yard_solve(yalgo);
    return 0;
}
