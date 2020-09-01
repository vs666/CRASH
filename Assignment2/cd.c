#include "cd.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TEST implementation of string handler for cd command

int findPath(char s[1000], String s4)
{
    int lev = 0;
    String s2 = strtok(s, "/");
    while (s2 != NULL)
    {
        if (!(s2[0] == '~' && strlen(s2) == 1))
        {
            push(s2);
            lev++;
        }
        s2 = strtok(NULL, "/");
    }

    s2 = strtok(s4, "/");

    while (s2 != NULL)
    {
        if (s2[0] == '.' && s2[1] == '.')
        {
            lev--;
            pop();
        }
        else if (!(s2[0] == '.' && strlen(s2) == 1))
        {
            lev++;
            push(s2);
        }
        s2 = strtok(NULL, "/");
    }
    
    return lev;
}
