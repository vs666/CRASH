#include "profork.h"
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
void runParellal(char *in)
{
    printf("\nParellal\n");
    char *arr[] = {
        "/bin/bash",
        "-c",
        in,
        NULL};
    int ff;
    if ((ff = fork()) == -1)
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
        return;
    }
}

void runSerial(char *in)
{
    printf("\nSerial\n");
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
        time(&t);
        printf("child started at %s", ctime(&t));
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
                time(&t);
                printf("child exited at %s", ctime(&t));
                if (WIFEXITED(status))
                    printf("\nProcess %s exited successfully.\n", arr[2]);
                else
                    fprintf(stderr, "\nchild process %s did not exit successfully\n", arr[2]);
                return;
            }
        } while (pid == 0);
    }
}