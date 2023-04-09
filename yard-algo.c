#include "yard-algo.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <assert.h>

void _buildExpression (struct yardAlgo*, const char*, const enum yardTokenType);

struct yardAlgo* yard_make ()
{
    struct yardAlgo* yarda = (struct yardAlgo*) malloc(sizeof(struct yardAlgo));
    yarda->expression = (struct yardToken*) malloc(0);
    yarda->symbols = (enum yardTokenType*) malloc(0);
    yarda->numsymbols = 0;
    yarda->sizexpr = 0;
    return yarda;
}

void yard_pushToken (struct yardAlgo* yarda, const char* token)
{
    assert(yarda);
    assert(token);

    switch (token[0]) {
        case '+': case '-':
        case '*': case '/': {
            yarda->symbols = (enum yardTokenType*) realloc(
                yarda->symbols,
                ++yarda->numsymbols * sizeof(enum yardTokenType)
            );
            yarda->symbols[yarda->numsymbols - 1] = token[0];
            break;
        }
        default: {
            _buildExpression(yarda, token, YARD_TOKEN_TYPE_NUM);
            break;
        }
    }
}

double yard_perform (struct yardAlgo* yarda)
{
    for (size_t i = 0; i < yarda->numsymbols; i++)
        _buildExpression(yarda, (char*) &yarda->symbols[i], yarda->symbols[i]);

    for (size_t i = 0; i < yarda->sizexpr; i++) {
        printf("%s, ", yarda->expression[i].data);
    }
    return 0;
}

void _buildExpression (struct yardAlgo* yarda, const char* data, const enum yardTokenType type)
{
    yarda->expression = (struct yardToken*) realloc(
        yarda->expression,
        ++yarda->sizexpr * sizeof(struct yardToken)
    );

    struct yardToken newt = {
        .data = (char*) malloc(strlen(data)),
        .type = type
    };
    strcpy(newt.data, data);;
    yarda->expression[yarda->sizexpr - 1] = newt;
}

int main ()
{
    struct yardAlgo *yardalgo = yard_make();
    char* expr[] = {
        "2", "+", "2"
    };

    for (int i = 0; i < 3; i++)
        yard_pushToken(yardalgo, expr[i]);
    yard_perform(yardalgo);
    return 0;
}
