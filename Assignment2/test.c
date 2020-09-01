#include "cd.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define MAX_FILENAME 1000

// TEST implementation of string handler for cd command

String findPath(char s[1000], String s4)
{
    // char s[1000] = "~/ocwa/secret_agents/agent_perry/profile";
    printf("Hello %s \n", s);
    fflush(stdout);
    String s2 = strtok(s, "/");
    while (s2 != NULL)
    {
        printf("%s\n", s2);
        if (!(s2[0] == '~' && strlen(s2) == 1))
        {
            push(s2);
        }
        s2 = strtok(NULL, "/");
    }

    // path stack is complete, now we can traverse the query

    // printf("Enter query string : ");
    // String s4 = (String)malloc(1000);
    // scanf("%s", s4);

    s2 = strtok(s4, "/");
    while (s2 != NULL)
    {
        if (strlen(s2) == 2 && s2[0] == '.' && s2[1] == '.')
        {
            pop();
        }
        else if (!(s2[0] == '.' && strlen(s2) == 1))
        {
            push(s2);
        }
        s2 = strtok(NULL, "/");
    }
    String s3 = (String)malloc(1000);

    s3[1] = '/';
    s3[0] = '~';
    for (int x = 0; x < size(); x++)
    {
        strcat(s3, ar__stack[x]);
        strcat(s3, "/");
    }

    // printf("final path is : %s\n", s3);
    return s3;
}
