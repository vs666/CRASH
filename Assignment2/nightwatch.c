#include <stdio.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include "kb.h"
void nw(int time, int command)
{
    char *fname = "/proc/interrupts";
    if (command == 0)
    {
        fname = "/proc/loadavg";
    }
    char *fileData = (char *)malloc(1000);
    size_t size = 1000;
    fflush(stdout);
    fflush(stdin);
    if (command == 1)
    {
        FILE *fp1;
        fp1 = fopen(fname, "r");
        getline(&fileData, &size, fp1);
        printf("%s", fileData);
        free(fp1);
    }

    char inp = 0;
    while (1)
    {
        FILE *fp;
        fp = fopen(fname, "r");
        if (command == 1)
        {
            getline(&fileData, &size, fp);
            getline(&fileData, &size, fp);
            getline(&fileData, &size, fp);

            int firstNo = 1;

            printf("%s", fileData);
        }
        else
        {
            char *sp = (char *)malloc(1000);
            getline(&sp, &size, fp);
            for (int x = strlen(sp); x >= 0; x--)
            {
                if (sp[x] == ' ')
                {
                    sp = &(sp[x + 1]);
                }
            }
            printf("%s\n", sp);
        }
        fflush(stdout);

        char cc = 0;
        // char *cc = (char *)malloc(10);
        fd_set ins;
        struct timeval to;
        int rrr = 0;
        int rb = 0;
        FD_ZERO(&ins);
        FD_SET(0, &ins);
        to.tv_sec = time;
        to.tv_usec = 0;
        rrr = select(1, &ins, NULL, NULL, &to);
        if (rrr)
        {

            cc = getchar();
            // read(0, &cc, 1);
            if (cc == 'q')
            {
                fflush(stdin);
                fflush(stdout);
                free(fp);
                free(fileData);
                return;
            }
        }
        free(fp);
    }
    free(fileData);
}
