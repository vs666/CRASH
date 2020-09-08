#include "profork.h"
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
void runParellal(char *in)
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
        return;
    }
    else if (ff == (pid_t)0)
    {
        setpgid(0, 0);
        execvp(arr[0], arr);
        exit(1);
    }
    else
    {
        printf("Child Process ID : %d %d\n", ff, getgid());
        tcsetpgrp(0, getgid());
        // return;
    }
    // printf("Exited process %s\n", arr[0]);
    // fflush(stdout);
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
            if ((pid = waitpid(pid, &status, WNOHANG)) == -1)
            {
                perror("wait() error");
                return;
            }
            else if (pid != 0)
            {
                if (!WIFEXITED(status))
                    fprintf(stderr, "\nchild process %s did not exit successfully\n", arr[2]);
                // else
                //     printf("\nProcess %s exited successfully.\n", arr[2]);
                return;
            }
        } while (pid == 0);
    }
}