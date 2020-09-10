#include <stdio.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

int islastword(char *s1, char *s2)
{
    int ml = strlen(s2);
    if (strlen(s1) < strlen(s2))
    {
        return 0;
    }
    else
    {
        int x = strlen(s1);
        while (x >= 1)
        {
            if (s1[x - 1] == ' ')
            {
                for (int y = 0; y < strlen(s2); y++)
                {
                    if (s2[y] != s1[x + y])
                    {
                        return 0;
                    }
                }
                return 1;
            }
            x--;
        }
        return 0;
    }
}

void nw(int time, int command)
{
    char *fname = "/proc/interrupts";
    if (command == 0)
    {
        fname = "/proc/loadavg";
    }
    int c = 0;
    char *fileData = (char *)malloc(1000);
    size_t size = 1000;
    fflush(stdout);
    fflush(stdin);
    if (command == 1)
    {

        FILE *fp1;
        fp1 = fopen(fname, "r");
        getline(&fileData, &size, fp1);
        char *tmp1 = strtok(fileData, " \t\n\0");

        while (tmp1 != NULL)
        {
            if (strlen(tmp1) != 0)
            {
                if (tmp1[strlen(tmp1) - 1] == '\n')
                {
                    printf("%s", tmp1);
                }
                else
                    printf("%s\t", tmp1);
                c++;
            }
            tmp1 = strtok(NULL, " \n\0\t");
        }

        free(tmp1);
        free(fp1);
        printf("\n");
    }

    char inp = 0;
    while (1)
    {
        FILE *fp;
        fp = fopen(fname, "r");
        if (command == 1)
        {

            do
            {
                getline(&fileData, &size, fp);
            } while (islastword(fileData, "i8042") == 0);

            int fc = 0;
            char *tmp1 = strtok(fileData, " \t");

            while (tmp1 != NULL)
            {
                if (fc > 0 && fc <= c)
                {
                    printf("%s\t", tmp1);
                }
                if (fc >= c)
                {
                    printf("\n");
                    break;
                }
                if (strlen(tmp1) > 0)
                    fc++;
                tmp1 = strtok(NULL, " \t");
            }
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
