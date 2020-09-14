#include "stack.h"
#include <stdlib.h>

// Code here

int top = 0;
String ar__stack[1000];

void push(String s)
{
    ar__stack[top] = (String)malloc(1000);
    ar__stack[top] = s;
    top++;
}

String pop()
{
    if (top == 0)
        return NULL;
    top--;
    return ar__stack[top];
}

int size()
{
    return top;
}
