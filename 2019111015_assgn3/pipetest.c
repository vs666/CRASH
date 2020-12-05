#include "includefiles.h"

int main()
{
    // Code to demonstrate inter process pipe communication

    int fd[2];

    if (pipe(fd) < 0)
    {
        perror("pipe ");
        exit(1);
    }
    int pid = fork();
    if (pid < 0)
    {
        perror("fork");
        exit(1);
    }
    if (pid == 0)
    {
        close(fd[1]);
        printf("[%d](child) > Waiting for content to be available.\n", getpid());
        char strRead[102];
        read(fd[0], strRead, 100);
        printf("[%d](child) > Contents of child : %s\n", getpid(), strRead);
        close(fd[0]);
    }
    else
    {
        close(fd[0]);
        sleep(2);
        printf("[%d](Parent) > ", getpid());
        char inp[102];
        scanf("%s", inp);
        write(fd[1], inp, 100);
        close(fd[1]);
        sleep(1);
    }
    printf("Exitting all processes\n");
}