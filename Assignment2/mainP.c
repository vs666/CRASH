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
#include "nightwatch.h"
#include "profork.h"
#include "pinfor.h"
#include "hist.h"
// Couldn't use execvp to call an executable +so making a function instead

void echo(char *message)
{
    int op = write(1, message, strlen(message));
    if (message[strlen(message) - 1] != '\n')
        write(1, "\n", 1);
    if (op != strlen(message))
    {
        perror("Echo : ");
    }
}

// simple procedure so single function is sufficient
/*
    The getcwd function is used to find the path to current directory
*/
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

int strcomp(char *s1, char *s2)
{
    int l = strlen(s2);
    if (strlen(s1) < l)
    {
        return 0;
    }
    for (int x = 0; x < l; x++)
    {
        if (s1[x] != s2[x])
        {
            return 0;
        }
    }
    return 1;
}

// main call
int main(int argc, char *argv[])
{

    // Stores the base directory path
    char *relp = (char *)malloc(1000);
    getcwd(relp, 1000);

    // get details for prompt-printing
    char *st = malloc(2000);
    int num;
    getLog();
    char *username = (char *)malloc(1000);
    getlogin_r(username, 1000);
    char *system_name = (char *)malloc(1000);
    gethostname(system_name, 1000);
    pid_t stac[1000];
    int stacsize = 0;
    while (1)
    {
        /*
         path stores the current path relative to ~ as the relp path
         This is done as the assignment specified that the direcotory from which the terminal is called is the base directory 
        */
        String path = (String)calloc(1000, 1);
        getcwd(path, 1000);
        strlen(relp);
        path[strlen(relp) - 1] = '~';
        char *mess = (char *)malloc(1000);
        path = &(path[strlen(relp) - 1]);
        int num = sprintf(st, "<\033[1;36m%s@\033[1;32m%s:\033[1;33m%s\033[0m> ", username, system_name, path);
        write(1, st, num);
        char *in = (char *)calloc(1000, 1);
        char *inall = (char *)calloc(1000, 1);
        read(0, inall, 1000);
        char *tmp_r_strtok = NULL;
        in = strtok_r(inall, ";", &tmp_r_strtok);
        while (in != NULL)
        {


            // check for the exitted background processes 
            
            for (int x = 0; x < stacsize; x++)
            {
                int *loc;
                int v = waitpid(stac[x], loc, WNOHANG);
                if (v > 0)
                {
                    printf("\nProcess %d exited with status %d\n", stac[x], *loc);
                }
            }
            // front trim code
            append_log(in);
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

            // changedir command
            // uses cd.c to check the directory is not below the ~ of our shell

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

            // if command is ls
            // provides -a and -l flags

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
                        in[x] = ' ';
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
                        in[x] = ' ';
                    }
                    else if (in[x] == '\n')
                    {
                        break;
                    }
                    // else
                    // {
                    //     // printf("Hello4 %d\n", x);
                    //     pathIndex = x;
                    //     break;
                    // }
                }

                in[0] = ' ';
                in[1] = ' ';
                int alone = 1;
                for (int x = 0; x < strlen(in); x++)
                {
                    if (in[x] == '\n')
                    {
                        in[x] = ' ';
                    }
                    if (in[x] != ' ' && in[x] != '\t')
                    {
                        alone = 0;
                    }
                }
                if (alone)
                {
                    in[0] = '.';
                }
                String s_fn = strtok(in, " \t\n");
                while (s_fn != NULL)
                {
                    printf("%s\n", s_fn);

                    if (strlen(s_fn) == 0)
                    {
                        s_fn = strtok(NULL, " \t\n\0");
                        continue;
                    }

                    if (s_fn[0] == '~' && strlen(s_fn) == 1)
                    {
                        list(flags, relp);
                    }
                    else
                    {
                        list(flags, s_fn);
                    }
                    s_fn = strtok(NULL, " \t\n\0");
                }
                free(s_fn);

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

            // proc that runs sequentially and parellelly
            // parellel processes need to be called be a & sign at the end
            else if (in[0] == 'p' && in[1] == 'i' && in[2] == 'n' && in[3] == 'f' && in[4] == 'o' && (strlen(in) == 5 || in[5] == '\n' || in[5] == ' '))
            {
                int index = -1;
                String s_pid = (String)calloc(1000, 1);
                for (int x = 5; x < strlen(in); x++)
                {
                    if (in[x] <= '9' && in[x] >= '0')
                    {
                        index = x;
                        break;
                    }
                }
                if (index == -1)
                {
                    sprintf(s_pid, "%d", getpid());
                }
                else
                {
                    strcpy(s_pid, &(in[index]));
                }
                pinformation(s_pid);
            }
            else if (in[0] == 'h' && in[1] == 'i' && in[2] == 's' && in[3] == 't' && in[4] == 'o' && in[5] == 'r' && in[6] == 'y' && (strlen(in) == 7 || in[7] == ' ' || in[7] == '\t' || in[7] == '\n'))
            {
                int xxc = 20;
                for (int x = 7; x < strlen(in); x++)
                {
                    if (in[x] <= '9' && in[x] >= '0')
                    {
                        if (in[x + 1] >= '0' && in[x] <= '9')
                            xxc = (in[x] - '0') * 10 + (in[x + 1] - '0');
                        else
                        {
                            xxc = in[x] - '0';
                        }
                    }
                }
                // printf("xxc : %d\n", xxc);
                if (xxc == 0)
                {
                    saveLog();
                }
                else
                    show_log(xxc);
            }
            else if (strcomp(in, "nightswatch"))
            {
                String st1 = (String)malloc(1000);
                strcpy(st1, in);
                String st2 = strtok(st1, " \n\t");
                int time_def = 1; // default time delay
                int sig = -1;
                while (st2 != NULL)
                {
                    if (st2[0] >= '0' && st2[0] <= '9')
                    {
                        time_def = atoi(st2);
                        // break;
                    }
                    if (strcomp(st2, "interrupt"))
                    {
                        sig = 1;
                    }
                    else if (strcomp(st2, "newborn"))
                    {
                        sig = 0;
                    }
                    st2 = strtok(NULL, " \t\n");
                }
                if (sig == -1)
                {
                    printf("\033[1;31m Error : Invalid command\033[0m\n");
                }
                else
                    nw(time_def, sig);
            }
            else
            {

                // Check if command is running from supposed construct or the bash command line

                // forking any process
                int seq = 1;

                for (int x = strlen(in); x > 0; x--)
                {
                    if (in[x] == ' ' || in[x] == '\t' || in[x] == '\n')
                    {
                        in[x] = ' ';
                    }
                    else
                    {
                        break;
                    }
                }
                for (int x = strlen(in) - 1; x > 0; x--)
                {
                    if (in[x] == '&' && in[x - 1] == ' ')
                    {
                        seq = 0;
                        in[x] = ' ';
                        break;
                    }
                }

                // using profork.h header file.

                if (seq == 0)
                    runParellal(in);
                else
                {
                    runSerial(in);
                }
            }
            in = strtok_r(NULL, ";", &tmp_r_strtok); // finds the next instruction
        }
    }
}
