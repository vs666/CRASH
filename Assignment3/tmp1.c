#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char *n = (char *)malloc(1000);
    read(0, n, 1000);
    char *t = strtok(n, ";");
    while (t != NULL)
    {
        printf("%s\n", t);
        t = strtok(NULL, ";");
    }
}