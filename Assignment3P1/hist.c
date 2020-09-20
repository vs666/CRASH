#include "hist.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

void getLog()
{
    FILE *fp;

    fp = fopen(TILDA, "r");
    if (fp == NULL)
    {
        printf("ERROR in loading command history. %s", TILDA);
    }
    int index = 0;
    char *st1 = (char *)malloc(1000);
    size_t ss = 1000;

    while (index <= 20 && fgets(st1, 1000, fp) != NULL)
    {
        index++;
        insert(st1);
        free(st1);
        st1 = (char *)malloc(1000);
    }
    fclose(fp);
}

void show_log(int n)
{
    displayNodes(n, NULL);
}

void append_log(char *command)
{
    insert(command);
}

void saveLog()
{
    FILE *fp;
    fp = fopen(TILDA, "w");
    if (fp == NULL)
    {
        printf("ERROR in saving command history.");
    }
    displayNodes(20, fp);
    fclose(fp);
}