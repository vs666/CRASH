#include "profork.h"
#include "includefiles.h"

int runParellal(char *in)
{
    char *arr[] = {
        "/bin/bash",
        "-c",
        in,
        NULL};
    pid_t ff;
    int status;
    pid_t pp;
    if ((ff = fork()) == -1)
    {
        pp = getpid();
        perror("fork : ");
        return -1;
    }
    else if (ff == (pid_t)0)
    {
        setpgrp();
        if (execvp(arr[0], arr) < 0)
        {
            perror("\nexecvp error ");
        }
    }
    else
    {
        printf("\n[+1 background process] %d (pid) \n", ff);
        return ff;
    }
}

int runSerial(char *in, int *fPid)
{
    int ex_code = -1;
    time_t t;
    char *arr[] = {
        "/bin/bash",
        "-c",
        in,
        NULL};
    int status;
    pid_t pid;
    if ((pid = fork()) < 0)
    {
        perror("fork : ");
        return -1;
    }
    else if (pid == 0)
    {

        if ((ex_code = execvp(arr[0], arr)) < 0)
        {
            perror("execvp(foreground) failed");
        }
    }
    else
    {
        *fPid = pid;
        waitpid(pid, &status, WUNTRACED);
        *fPid = -1;
        return status;
    }
}