#include "profork.h"
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

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

void runSerial(char *in)
{
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
        return;
    }
    else if (pid == 0)
    {
        execvp(arr[0], arr);
        exit(1);
    }
    else
    {
        do
        {
            if ((pid = waitpid(pid, &status, WNOHANG)) == -1 && errno != 10) // check how to seperate ERRNO = 10 of parellal process from serial process
            {
                fflush(stdout);
                perror("wait() error");
                return;
            }
            else if (pid > 0)
            {
                if (!WIFEXITED(status))
                    fprintf(stderr, "\nchild process %s did not exit successfully\n", arr[2]);
                return;
            }
        } while (pid == 0);
    }
}