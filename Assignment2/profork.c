#include "profork.h"
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void runParellal(char *in)
{
    char *arr[] = {
        "/bin/bash",
        "-c",
        in,
        NULL};
    int ff = fork();
    if (ff == -1)
    {
        perror("fork : ");
        return;
    }
    if (ff == 0)
    {
        execvp(arr[0], arr);
        exit(0);
    }
    else
    {
        return;
    }
}

void runSerial(char *in)
{
    char *arr[] = {
        "/bin/bash",
        "-c",
        in,
        NULL};
    printf("%s\n", in);
    int ff = fork();
    if (ff == -1)
    {
        perror("fork : ");
        return;
    }
    else if (ff == 0)
    {
        execvp(arr[0], arr);
        exit(0);
    }
    else
    {
        // printf("%s\n", in);
        printf("Process opened \n");
        wait(NULL);
        return;
    }
}