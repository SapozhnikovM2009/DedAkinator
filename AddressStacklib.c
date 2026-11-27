#include "AddressStacklib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#define POISON (TreeNode_t*)-24012009
#define CANARY1 (TreeNode_t*)0xBADC0FFEE
#define CANARY2 (TreeNode_t*)0xDEADF00D
#define STACKCANARY1 0xC001BEEF
#define STACKCANARY2 0xBADEDA



int StackInit(stack* stk, int capacity)
{
    stk->data = calloc(capacity + 2, sizeof(TreeNode_t*));
    //cvt(stk->data)[2] = 1;
    if (stk->data == NULL)
    {
        perror("calloc");
        return -1;
    }
    stk->data[0] = CANARY1;
    stk->data[capacity + 1] = CANARY2;
    stk->capacity = capacity;
    stk->size = 0;
    for (int i = 1;i < capacity + 1;i++)
    {
        stk->data[i] = POISON;
    }
    stk->StackCanary1 = STACKCANARY1;
    stk->StackCanary2 = STACKCANARY2;

    #ifndef NDEBUG
        Verify(stk);
    #endif
    
    return 0;
}

void Verify(stack* stk)
{
    if (stk == NULL)
    {
        printf(RED BOLD "\nStack wrong address!\n" BLACK);
        Dump(stk);
        return;
    }

    if (stk->data == NULL)
    {
        printf(RED BOLD "\nStackData wrong address!\n" BLACK);
        Dump(stk);
        return;
    }

    if (stk->size < 0 || stk->size > stk->capacity)
    {
        printf(RED BOLD "\nSize is wrong!\n" BLACK);
        Dump(stk);
        return;
    }

    if (stk->capacity <= 0)
    {
        printf(RED BOLD "\nCapacity is wrong!\n" BLACK);
        Dump(stk);
        return;
    }

    if (stk->StackCanary1 != STACKCANARY1 || stk->StackCanary2 != STACKCANARY2)
    {
        printf(RED BOLD "\nStackCanary is wrong!\n" BLACK);
        Dump(stk);
        return;
    }

    if (stk->data[0] != CANARY1 || stk->data[stk->capacity + 1] != CANARY2)
    {
        printf(RED BOLD "\nData Canary is wrong!\n" BLACK);
        Dump(stk);
        return;
    }

}

void Dump(stack* stk)
{
    for (int i = 0;i < 35;i++) printf(CYAN "-" BLACK);
    if (stk->size > stk->capacity || stk->size < 0)
    {
        printf(CYAN "\n|" RED BOLD "  size = " RED BOLD "%d" BLACK "\t", stk->size);
    }
    else
    {
        printf(CYAN "\n|" BLACK "  size = " GREEN BOLD "%d" BLACK "\t", stk->size);
    }


    if (stk->capacity <= 0 || stk->size > stk->capacity)
    {
        printf(RED BOLD "capacity = %d" BLACK "\n", stk->capacity);
    }
    else
    {
        printf("capacity = " GREEN BOLD "%d" BLACK "\n", stk->capacity);
    }


    if (stk->data != NULL)
    {
        printf(CYAN "|" BLACK "  Stack data address: " GREEN BOLD "%p" BLACK "\n", stk->data);
    }
    else
    {
        printf(CYAN "|" RED BOLD "  Stack data address: %p" BLACK "\n", stk->data);
    }


    if (stk == NULL)
    {
        printf(CYAN "|" RED BOLD "  Stack address: %p" BLACK "\n" CYAN "|" BLACK "\n", stk);
    }
    else
    {
        printf(CYAN "|" BLACK "  Stack address: " GREEN BOLD "%p" BLACK "\n" CYAN "|" BLACK "\n", stk);
    }


    if (stk->StackCanary1 == STACKCANARY1)
    {
        printf(CYAN "|" BLACK "  StackCanary1 : " GREEN BOLD "%ld" BLACK "\n", stk->StackCanary1);
    }
    else
    {
        printf(CYAN "|" RED BOLD "  StackCanary1 : %ld" BLACK "\n", stk->StackCanary1);
    }


    if (stk->StackCanary2 == STACKCANARY2)
    {
        printf(CYAN "|" BLACK "  StackCanary2 : " GREEN BOLD "%ld" BLACK "\n", stk->StackCanary2);
    }
    else
    {
        printf(CYAN "|" RED BOLD "  StackCanary2 : %ld" BLACK "\n", stk->StackCanary2);
    }


    printf( CYAN "|" BLACK "\n");

    if (stk->data[0] == CANARY1) printf(CYAN "|" GREEN BOLD "  Canary1 : " BLACK "  %p" BLACK, stk->data[0]);
    else printf(CYAN "|" RED BOLD "  Canary1 : " BLACK "  %p" RED BOLD " (WARNING)" BLACK, stk->data[0]);
    printf("\n");


    for (int i = 1;i < stk->size + 1;i++) printf(CYAN "|" BLACK "\t" LIGHT_BLUE BOLD " [" BLACK "%d" LIGHT_BLUE BOLD "]" BLACK " = %p\n", i, stk->data[i]);
    for (int i = stk->size + 1;i < stk->capacity + 1;i++)
    {
        printf(PURPLE "|" BLACK "\t" RED BOLD "*" LIGHT_BLUE BOLD "[" BLACK "%d" LIGHT_BLUE BOLD "] " BLACK "= %p", i, stk->data[i]);
        if (stk->data[i] == POISON) printf(GREEN BOLD " (POISON)" BLACK);
        printf("\n");
    }


    if (stk->data[stk->capacity + 1] == CANARY2) printf(PURPLE "|" GREEN BOLD "  Canary2 : " BLACK "  %p", stk->data[stk->capacity + 1]);
    else printf(PURPLE "|" RED BOLD "  Canary2 : " BLACK "  %p" RED BOLD " (WARNING)" BLACK, stk->data[stk->capacity + 1]);
    printf("\n");

    for (int i = 0;i < 35;i++) printf(CYAN "-" BLACK);
    printf("\n");
}

void StackPush(stack* stk, TreeNode_t *elem)
{
    #ifndef NDEBUG
        Verify(stk);
    #endif

    if (stk->size == stk->capacity)
    {
        int new_capacity = stk->capacity * 2;
        TreeNode_t **p = realloc(stk->data, sizeof(TreeNode_t*) * (new_capacity + 2)); // +2 for canary передвинуть 2 канарнейку
        if (p != NULL)
        {
            stk->data = p;
            stk->capacity = new_capacity;
            stk->data[stk->capacity + 1] = CANARY2;
            //Dump(stk);
            for (int i = stk->size + 1;i < new_capacity + 1;i++)
            {
                stk->data[i] = POISON;
            }
        }
        else
        {
            perror("realloc");
            return;
        }
    }
    stk->data[stk->size + 1] = elem;
    stk->size++;

    #ifndef NDEBUG
        Verify(stk);
    #endif

}

TreeNode_t *StackPop(stack* stk)
{
    #ifndef NDEBUG
        Verify(stk);
    #endif

    // assert(stk->size > 0)
    // assert(0)
    if (stk->size == 0)
    {
        Dump(stk);
        printf("Pop from null stack\n");
        return NULL;
    }
    TreeNode_t *SaveElem = stk->data[stk->size];
    stk->data[stk->size] = POISON;
    stk->size--;

    #ifndef NDEBUG
        Verify(stk);
    #endif

    return SaveElem;
}

void StackDestroy(stack* stk)
{
    free(stk->data);
}

/*
5*) Хеши 
//ассерты проэкспериментировать (начать использовать)
разобраться с ifndef... посмотреть возможные конструкции
*/
