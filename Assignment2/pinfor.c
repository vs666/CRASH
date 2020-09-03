#include "pinfor.h"
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

void pinformation(String nm1)
{
    char *nm = (String)malloc(1000);
    for (int x = 0; x < strlen(nm1); x++)
    {
        if (nm1[x] == ' ' || nm1[x] == '\t' || nm1[x] == '\n')
        {
            break;
        }
        else
        {
            nm[x] = nm1[x];
        }
    }
    String path1 = (String)malloc(1000);
    strcat(path1, "/proc/");
    String path2 = (String)malloc(1000);
    strcat(path2, "/proc/");
    fflush(stdout);
    strcat(path1, nm);
    strcat(path2, nm);
    FILE *fp;
    char status = '-';

    String exe_name = (String)malloc(1000);
    String mem_size = (String)malloc(1000);
    String pid_line = (String)malloc(1000);
    strcat(path1, "/status");
    strcat(path2, "/comm");
    fp = fopen(path1, "r");
    if (fp == NULL)
    {
        printf("\033[1m;33 ERROR : Internal Error \033[0m\n");
        return;
    }
    String line = (String)malloc(1000);
    int i = 0;
    size_t sz = 1000;
    while (i != 3 && getline(&line, &sz, fp) != 0)
    {
        if (line[0] == 'S' && line[1] == 't' && line[2] == 'a')
        {
            status = line[7];
            i++;
        }
        if (line[0] == 'V' && line[1] == 'm' && line[2] == 'S' && line[3] == 'i')
        {
            i++;
            strcpy(mem_size, &line[12]);
        }
        if (line[0] == 'P' && line[1] == 'i' && line[2] == 'd' && line[3] == ':')
        {
            i++;
            strcpy(pid_line, line);
        }
    }

    fp = fopen(path2, "r");
    if (fp == NULL)
    {
        printf("\033[1m;33 ERRORss : Internal Error \033[0m\n");
        return;
    }
    getline(&exe_name, &sz, fp);
    printf("%s", pid_line);
    printf("Process Status :        %c\n", status);
    printf("Memory :                %s", mem_size);
    printf("Executable Path :       %s\n", exe_name);
    return;
}
