#include "yard-algo.h"

size_t __stacksize;
size_t __queuesize;
bool __mustbe_number;

struct yardExpr* yardalgo_init ()
{
    struct yardExpr* yarde = (struct yardExpr*) malloc(sizeof(struct yardExpr));
    yarde->nums = (double*) malloc(0);
    yarde->symbols = (enum yardTokenType*) malloc(0);
    yarde->value = 0;

    __stacksize = 0;
    __queuesize = 0;
    __mustbe_number = true;
    return yarde;
}

bool yardalgo_pushToken (struct yardExpr* yarde, char* data)
{
    assert(yarde);
    assert(data);

    if (__mustbe_number) {
        yarde->nums = (double*) realloc(yarde->nums, ++__queuesize * sizeof(double));
        yarde->nums[__queuesize - 1] = strtod(data, NULL);

        __mustbe_number = false;
        return true;
    }
    else {
        yarde->symbols = (enum yardTokenType*) realloc(yarde->symbols, ++__stacksize * sizeof(enum yardTokenType));
        enum yardTokenType type;
        switch (data[0]) {
            case '+' : { type = YARD_TOKEN_TYPE_ADD_OP; break; }
            case '~' : { type = YARD_TOKEN_TYPE_SUB_OP; break; }
            case '/' : { type = YARD_TOKEN_TYPE_DIV_OP; break; }
            case '*' : { type = YARD_TOKEN_TYPE_MUL_OP; break; }
            case '(' : { type = YARD_TOKEN_TYPE_B_PARN; break; }
            case ')' : { type = YARD_TOKEN_TYPE_E_PARN; break; }
        }

        yarde->symbols[__stacksize - 1] = type;
        __mustbe_number = true;
        return true;
    }

    return false;
}

void yardalgo_perform (struct yardExpr* yarde)
{
    for (int i = 0; i < __queuesize; i++)
        printf("%f, ", yarde->nums[i]);
    for (int i = 0; i < __stacksize; i++)
        printf("%c, ", yarde->symbols[i]);

    for (size_t i = __stacksize - 1; i != -1; i--) {
        enum yardTokenType op = yarde->symbols[i];
        double nums[3] = {
            yarde->nums[i],
            yarde->nums[i + 1],
            0
        };
        switch (op) {
            case YARD_TOKEN_TYPE_ADD_OP : { nums[2] = nums[0] + nums[1]; break; }
            case YARD_TOKEN_TYPE_SUB_OP : { nums[2] = nums[0] - nums[1]; break; }
            case YARD_TOKEN_TYPE_DIV_OP : { nums[2] = nums[0] / nums[1]; break; }
            case YARD_TOKEN_TYPE_MUL_OP : { nums[2] = nums[0] * nums[1]; break; }
        }

        printf("\nCurrent: %f\n", nums[2]);
        yarde->value = nums[2];
        yarde->nums[i] = nums[2];
    }
    printf("\n%f\n", yarde->value);
}

int main () {
    char* x[] = {
        "2", "~", "6", "*", "3"
    };

    struct yardExpr* expr = yardalgo_init();
    for (int i = 0; i < 5; i++)
        yardalgo_pushToken(expr, x[i]);
    yardalgo_perform(expr);
    return 0;
}
