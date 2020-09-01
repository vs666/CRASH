#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "stack.h"
#include "cd.h"
#include "ls.h"
#include "profork.h"
// Couldn't use execvp to call an executable +so making a function instead
void echo(char *message)
{
    int op = write(1, message, strlen(message));
    if (op != strlen(message))
    {
        perror("Echo : ");
    }
}

// simple procedure so single function is sufficient
void pwd()
{
    char *st1 = malloc(1000);
    char *st2 = malloc(1000);
    getcwd(st1, 1000);
    int n = sprintf(st2, "%s\n", st1);
    write(1, st2, n);
    free(st1);
    free(st2);
}

// calls function in list.h
void list(String flags, String path)
{
    // printf("%s %s \n", flags, path);
    ls_main(flags, path);
}

// calls function in cd.h
int changedir(char c_path[1000], char *query)
{
    int final_path = findPath(c_path, query);
    return final_path;
}

// main call
int main(int argc, char *argv[])
{
    char *relp = (char *)malloc(1000);
    getcwd(relp, 1000);
    char *st = malloc(2000);
    int num;
    char *username = (char *)malloc(1000);
    getlogin_r(username, 1000);
    char *system_name = (char *)malloc(1000);
    gethostname(system_name, 1000);
    while (1)
    {
        String path = (String)calloc(1000, 1);
        getcwd(path, 1000);
        strlen(relp);
        path[strlen(relp) - 1] = '~';
        char *mess = (char *)malloc(1000);
        path = &(path[strlen(relp) - 1]);
        int num = sprintf(st, "<\033[1;36m%s@\033[1;32m%s:\033[1;33m%s\033[0m> ", username, system_name, path);
        write(1, st, num);
        char *in = (char *)malloc(1000);
        read(0, in, 1000);

        // front trim code
        for (int x = 0; x < 1000; x++)
        {
            if (in[x] == 32 || in[x] == '\t')
            {
                continue;
            }
            else
            {
                in = &(in[x]);
                break;
            }
        }

        if (in[0] == 'e' && in[1] == 'c' && in[2] == 'h' && in[3] == 'o' && in[4] == ' ')
        {
            echo(&(in[5]));
        }

        else if (in[0] == 'p' && in[1] == 'w' && in[2] == 'd')
        {
            pwd();
        }
        else if (in[0] == 'c' && in[1] == 'd' && (in[2] == ' ' || in[2] == '\t'))
        {
            // change directory code implementation
            int min_index = 0;
            for (int x = 3; x < strlen(in); x++)
            {
                if (in[x] != ' ' && in[x] != '\t')
                {
                    min_index = x;
                    break;
                }
            }

            if (strlen(path) == 1 && path[0] == '~' && in[min_index] == '.' && in[min_index + 1] == '.')
            {
                String ptz = (String)malloc(1000);
                getcwd(ptz, 1000);
                printf("%s\n", ptz);
                free(ptz);
            }
            else
            {
                String pt2 = (String)calloc(1000, 1);
                String pt3 = (String)calloc(1000, 1);
                strcpy(pt2, path);
                pt3[0] = '.';
                pt3[1] = '/';
                for (int x = min_index; x < strlen(in); x++)
                {
                    if (in[x] == '\n' || in[x] == ' ' || in[x] == '\t')
                    {
                        break;
                    }
                    else
                    {
                        pt3[2 + x - min_index] = in[x];
                    }
                }
                String pt4 = (String)calloc(1000, 1);
                strcpy(pt4, pt3);
                int pt = changedir(pt2, pt3);
                if (pt >= 0)
                {
                    int err = chdir(pt4);
                    if (err)
                        perror("chdir");
                }
                else
                {
                    printf("\033[1;31m");
                    printf("Error : Cannot access lower than ~/\n");
                    printf("\033[0m");
                    fflush(stdout);
                }
                free(pt3);
                free(pt2);
            }
        }
        else if (in[0] == 'l' && in[1] == 's' && (in[2] == ' ' || in[2] == '\t' || in[2] == '\n'))
        {
            char flags[100];
            flags[0] = '-';
            int pathIndex = -1;
            int fl = 0;
            int index = 1;
            for (int x = 2; x < strlen(in) - 1; x++)
            {
                // printf("fl : %d\n",fl);
                if (in[x] == '-' && in[x - 1] == ' ' && in[x + 1] != ' ')
                {
                    // printf("Hello1 %d\n", x);
                    fl = 1;
                }
                else if (in[x] == ' ' || in[x] == '\t')
                {
                    // printf("Hello2 %d\n", x);
                    fl = 0;
                }
                else if (fl == 1)
                {
                    // printf("Hello3 %d\n", x);
                    flags[index++] = in[x];
                }
                else if (in[x] == '\n')
                {
                    break;
                }
                else
                {
                    // printf("Hello4 %d\n", x);
                    pathIndex = x;
                    break;
                }
            }

            if (pathIndex == -1)
            {
                list(flags, ".");
            }
            else
            {
                int tt = 0;
                String s2ls = (String)calloc(100, 1);
                if (in[pathIndex] == '.' && in[pathIndex + 1] == '/')
                {
                    pathIndex += 2;
                }
                if (in[pathIndex] == '~' && in[pathIndex + 1] == '\n')
                {
                    strcpy(s2ls, relp);
                }
                else
                {
                    for (int x = pathIndex; x < strlen(in); x++)
                    {
                        if (in[x] == '\n')
                        {
                            in[x] = '\0';
                        }
                        if (in[x] == ' ' || in[x] == '\t')
                        {
                            break;
                        }
                        s2ls[tt++] = in[x];
                    }
                }
                // printf("Path passed : %s\n", s2ls);
                list(flags, s2ls);
                free(s2ls);
            }
            for (int x = 0; x < 100; x++)
            {
                if (flags[x] == 0)
                {
                    break;
                }
                else
                {
                    flags[x] = 0;
                }
            }
        }
        else
        {
            // forking any process
            for (int x = strlen(in); x > 0; x--)
            {
                if (in[x] == ' ' || in[x] == '\t' || in[x] == '\n')
                {
                    in[x] = '\0';
                }
                else
                {

                    break;
                }
            }

            int seq = 0;
            if (in[0] == 'v' && in[1] == 'i' && (in[2] == ' ' || (in[2] == 'm' && in[3] == ' ')))
            {
                seq = 1;
            }
            if (seq == 0)
                runParellal(in);
            else
            {
                runSerial(in);
            }
        }
        // free(path);
    }
}