#ifndef ADDRESSSTACKLIB_H
#define ADDRESSSTACKLIB_H

#include "treelib.h"

typedef struct
{
    long StackCanary1;
    TreeNode_t **data;
    int size; //size_t //ssize_t со знаком
    int capacity;
    long StackCanary2;
} stack;

#define RED "\033[31m"
#define BLACK "\033[0m"
#define CYAN "\033[36m"
#define BOLD "\033[1m"
#define PURPLE "\033[35m"
#define GREEN "\033[32m"
#define LIGHT_BLUE "\033[36m"

int StackInit(stack* stk, int capacity);
void Dump(stack* stk);
void StackPush(stack* stk, TreeNode_t *elem);
TreeNode_t *StackPop(stack* stk);
void Verify(stack* stk);
void StackDestroy(stack* stk);

#endif
