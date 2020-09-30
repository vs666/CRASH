#include "includefiles.h"
#include "stack.h"
#include "cd.h"
#include "ls.h"
#include "constants.h"
#include "nightwatch.h"
#include "profork.h"
#include "pinfor.h"
#include "hist.h"

// DEFINED CONSTANTS

// Couldn't use execvp to call an executable +so making a function instead

int p_exitID = 0;

int fPid = -1;
char *relp;
char *lastDir[2];
pid_t stac[1000];     // contains all the processes
pid_t stac2[1000];    // contains all the processes
int stacsize = 0;     // size of the stac
char *namestac[1000]; // name of process

char *trimString(char *str)
{
    char *str1 = (char *)calloc(LIST_LEN, 1);
    int x = 0, c = 0;

    while (x < strlen(str))
    {
        if (str[x] == '\0' || str[x] == EOF)
        {
            break;
        }
        if (str[x] == '\n' || str[x] == '\t')
        {
            str[x] = ' ';
        }
        if (str[x] == ' ' && (c == 0 || (x >= 1 && str[x - 1] == ' ')))
        {
            x++;
        }
        else
        {
            str1[c++] = str[x];
            x++;
        }
    }
    str = str1;
    return str1;
}

void handler(int sig)
{
    int x;
    pid_t pid = waitpid(-1, &x, WNOHANG);
    if (pid > 0)
    {
        if (WIFEXITED(x) | WIFSIGNALED(x))
        {
            char *processName = (char *)malloc(1000);
            for (int xx = 0; xx < stacsize; xx++)
            {
                if (pid == stac[xx])
                {
                    strcpy(processName, namestac[xx]);
                    stac[xx] = -1;
                }
            }
            processName = trimString(processName);
            fprintf(stderr, "\n");
            if (WEXITSTATUS(x) == EXIT_SUCCESS)
            {
                fprintf(stderr, "Process %s exited abnormally with status %d", processName, WEXITSTATUS(x));
            }
            else
            {
                fprintf(stderr, "Process %s exited normally", processName);
            }
            free(processName);
            fprintf(stderr, "\n");
            fflush(stderr);
        }
    }
}
void killProcAll()
{
    for (int x = 0; x < stacsize; x++)
    {
        if (stac[x] != -1)
            kill(stac[x], SIGKILL);
    }
}
void closeShell()
{
    printf("^D\n");
    printf("Closing all processes\n");
    saveLog(relp);
    killProcAll();
    printf("Exitting Shell\n");
    fflush(stdout);

    exit(0);
}
void exitHandler(int sign)
{
    printf("EXIT HANDLER!!");
    char c;
    signal(sign, SIG_IGN);
    closeShell();
}
void echo(char *message)
{
    int co = 0;
    if (message[strlen(message) - 1] != '\n')
        co = 1;
    char *mes = strtok(message, " ");
    while (mes != NULL)
    {
        printf("%s", mes);
        // fprintf(stdout, mes);
        if (mes[strlen(mes) - 1] != '\n')
        {
            printf(" ");
            // fprintf(stdout, " ");
        }
        mes = strtok(NULL, " ");
    }
    if (co)
    {
        printf("\n");
        // printf("\n");
    }
}
/*
    The getcwd function is used to find the path to current directory
*/
void listJobs()
{

    int count = 1;
    for (int x = 0; x < stacsize; x++)
    {
        if (stac[x] != -1)
        {
            char *s1 = (char *)malloc(LIST_LEN);
            sprintf(s1, "/proc/%d/stat", stac[x]);
            FILE *nf = fopen(s1, "r");
            if (nf == NULL)
            {
                continue;
            }
            size_t size = 1000;
            char status = 0;
            getline(&s1, &size, nf);
            for (int x = 0; x < strlen(s1); x++)
            {
                if (s1[x] == ')' && s1[x + 1] == ' ')
                {
                    if (s1[x + 2] == 'S')
                    {
                        status = 1;
                    }
                    break;
                }
            }
            printf("[%d]\t%s\t\t%s\t[%d]\n", count, namestac[x], (status == 1 ? "Running" : "Stopped"), stac[x]);
            count++;
        }
    }
    if (count == 1)
    {
        printf("No jobs to show.\n");
    }
}
void pwd()
{
    char *st1 = malloc(1000);
    char *st2 = malloc(1000);
    getcwd(st1, 1000);
    int n = sprintf(st2, "%s\n", st1);
    printf("%s", st2);
    free(st1);
    free(st2);
}
// calls function in list.h
void list(String flags, String path)
{
    // printf("%s %s \n", flags, path);
    p_exitID = ls_main(flags, path);
}
// calls function in cd.h
int changedir(char c_path[1000], char *query)
{
    int final_path = findPath(c_path, query);
    return final_path;
}
// check if s2 is a prefix of s1
int strcomp(char *s1, char *s2)
{
    int l = strlen(s2);
    if (strlen(s1) < l)
    {
        return 0;
    }
    for (int x = 0; x < l; x++)
    {
        if (s1[x] != s2[x] && s1[x] != '\n')
        {
            return 0;
        }
    }
    return 1;
}
// main call
void handleZ(int sig)
{
    if (fPid != -1 && fPid != getpid())
    {
        printf("Stopped process pid %d\n", fPid);
        kill(fPid, SIGTSTP);
        stac[stacsize] = fPid;
        stac2[stacsize] = fPid;
        char *tmp_file = (char *)malloc(LIST_LEN);
        sprintf(tmp_file, "/proc/%d/comm", fPid);
        tmp_file = trimString(tmp_file);
        char *tmp_command = (char *)malloc(LIST_LEN);
        read(open(tmp_file, O_RDWR), tmp_command, LIST_LEN);
        tmp_command = trimString(tmp_command);
        namestac[stacsize] = tmp_command;
        stacsize++;
        fPid = -1;
        raise(SIGCONT);
    }
    // fprintf(stderr, "%d is fPid and signal is %d\n", fPid, sig);
    // if (fPid > 0 && fPid != getpid())
    // {
    //     fprintf(stderr, "%d is fPid %d is tpid\n", fPid, getpid());

    //     // if (kill(fPid, SIGSTOP) != 0 || kill(getpgid(fPid), SIGCONT) != 0)
    //     // {
    //     //     perror("raise(SIGSTOP)");
    //     // }
    //     if (raise(SIGTSTP) != 0)
    //     {
    //         perror("raise(SIGTSTP)");
    //     }
    //     else
    //     {
    //         fprintf(stderr, "%d PID is pushed to background\n", fPid);
    //         fPid = -1;
    //     }
    // }
}

void execCommand(char *in, char *path)
{
    if (in[0] == 'e' && in[1] == 'c' && in[2] == 'h' && in[3] == 'o' && in[4] == ' ')
    {
        echo(&(in[5]));
    }
    else if (in[0] == 'p' && in[1] == 'w' && in[2] == 'd')
    {
        pwd();
    }
    else if (strcomp(in, "fg "))
    {
        char *tmpzz;
        char *nn = strtok_r(in, " \n", &tmpzz);
        nn = strtok_r(NULL, " \n", &tmpzz);
        if (nn == NULL)
        {
            p_exitID = ERR_INVALID;
            printf("ERROR : Invalid Command\n");
            return;
        }
        int inNo = atoi(nn);
        int find = 0;
        for (int x = 0; x < stacsize; x++)
        {
            if (stac[x] == -1)
            {
                continue;
            }
            if (inNo != 1)
            {
                inNo--;
                continue;
            }
            find = 1;
            fPid = stac[x];
            stac[x] = -1;
            pid_t pid = fPid;
            kill(pid, SIGCONT);
            int status_code;
            waitpid(pid, &status_code, WUNTRACED);

            if (WIFSTOPPED(status_code))
            {
                fPid = -1;
            }
            if (WIFEXITED(status_code))
            {
                stac[x] = -1;
                fPid = -1;
                printf("Exitted\n");
            }
            break;
        }
        if (find == 0)
        {
            fprintf(stderr, "No such job found.\n");
            p_exitID = ERR_404;
        }
    }
    else if (in[0] == 'c' && in[1] == 'd' && (in[2] == ' ' || in[2] == '\t'))
    {
        // change directory code implementation
        if (in[3] == '-' && (strlen(in) == 4 || in[4] == '\t' || in[4] == ' ' || in[4] == '\n' || in[4] == '\0'))
        {
            printf("%s\n", lastDir[1]);
            chdir(lastDir[1]);
            p_exitID = errno;
            return;
        }
        int min_index = 0;
        for (int x = 3; x < strlen(in); x++)
        {
            if (in[x] != ' ' && in[x] != '\t')
            {
                min_index = x;
                break;
            }
        }

        String pt2 = (String)calloc(1000, 1);
        String pt3 = (String)calloc(1000, 1);
        strcpy(pt2, path);
        for (int x = min_index; x < strlen(in); x++)
        {
            if (in[x] == '\n' || in[x] == ' ' || in[x] == '\t')
            {
                break;
            }
            else
            {
                pt3[x - min_index] = in[x];
            }
        }
        if (strcomp(pt3, "~"))
        {
            String pttmp = (String)malloc(1000);
            strcpy(pttmp, &pt3[1]);
            strcpy(pt3, relp);
            strcat(pt3, pttmp);
            free(pttmp);
        }
        String pt4 = (String)calloc(1000, 1);
        strcpy(pt4, pt3);
        // int pt = changedir(pt2, pt3);
        int err = chdir(pt4);
        if (err)
        {
            printf("%d is err\n", err);
            perror("chdir");
            p_exitID = errno;
        }

        free(pt3);
        free(pt2);
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
                fl = 0;
            }
            else if (fl == 1)
            {

                flags[index++] = in[x];
                in[x] = ' ';
            }
            else if (in[x] == '\n')
            {
                break;
            }
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
    else if (strcomp(in, "exit") || strcomp(in, "quit"))
    {
        saveLog(relp);
        closeShell();
    }
    else if (strcomp(in, "jobs"))
    {
        listJobs();
    }

    // proc that runs sequentially and parellelly
    // parellel processes need to be called be a & sign at the end

    else if (in[0] == 'p' && in[1] == 'i' && in[2] == 'n' && in[3] == 'f' && in[4] == 'o' && (strlen(in) == 5 || in[5] == '\n' || in[5] == ' '))
    {
        int index = -1;
        int errorcode = 0;
        String s_pid = (String)calloc(1000, 1);
        for (int x = 5; x < strlen(in); x++)
        {
            if (in[x] <= '9' && in[x] >= '0')
            {
                index = x;
                break;
            }
            else if (in[x] != ' ' && in[x] != '\t' && in[x] != '\n')
            {
                errorcode = 1;
                break;
            }
        }
        if (errorcode == 1)
        {
            p_exitID = ERR_INVALID;
            printf("\nInvalid argument pinfo\n");
        }
        else if (index == -1)
        {
            sprintf(s_pid, "%d", getpid());
        }
        else
        {
            strcpy(s_pid, &(in[index]));
        }
        pinformation(s_pid);
    } // HANDLING OF EXITTED PROCESSES

    else if (in[0] == 'h' && in[1] == 'i' && in[2] == 's' && in[3] == 't' && in[4] == 'o' && in[5] == 'r' && in[6] == 'y' && (strlen(in) == 7 || in[7] == ' ' || in[7] == '\t' || in[7] == '\n'))
    {
        int xxc = 10;
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
        if (xxc == 0)
        {
            saveLog(relp);
        }
        else
            show_log(xxc);
    }
    else if (strcomp(in, "overkill"))
    {
        killProcAll();
    }
    else if (strcomp(in, "getenv"))
    {
        char varName[LIST_LEN];
        char *tmp1;
        char *token = strtok_r(in, " \n", &tmp1);
        token = strtok_r(NULL, " \n", &tmp1);
        printf("%s is var\n", token);
        if (token == NULL || strtok_r(NULL, " \n", &tmp1) != NULL)
        {
            p_exitID = ERR_INVALID;
            fprintf(stderr, "Invalid number of arguments passed.");
        }
        else if (getenv(token) == NULL)
        {
            p_exitID = ERR_404;
            printf("Variable %s not found.\n", varName);
        }
        else
        {
            printf("%s : %s\n", token, getenv(token));
        }
    }
    else if (strcomp(in, "setenv"))
    {
        char value[LIST_LEN];
        int c = -1;
        for (int x = 0; x < strlen(in); x++)
        {
            if (c != -1 && in[x] == '"')
            {
                break;
            }
            else if (c == -1 && in[x] == '"')
            {
                c++;
            }
            else if (c != -1)
            {
                value[c++] = in[x];
            }
        }
        char *tmp1;
        int count = 0;
        char varName[LIST_LEN];
        char *token = strtok_r(in, " \t\n", &tmp1);
        while (token != NULL)
        {
            count++;
            if (count == 2)
            {
                strcpy(varName, token);
            }
            token = strtok_r(NULL, " \t\n", &tmp1);
        }
        if (count != 3 || c == -1)
        {
            p_exitID = ERR_INVALID;
            fprintf(stderr, "Invalid Number of arguments passed\n");
        }
        else
            setenv(varName, value, 1);
    }
    else if (strcomp(in, "unsetenv"))
    {
        char varName[LIST_LEN];
        char *tmp1;
        char *token = strtok_r(in, " \n", &tmp1);
        token = strtok_r(NULL, " \n", &tmp1);
        if (token == NULL || strtok_r(NULL, " \n", &tmp1) != NULL)
        {
            p_exitID = ERR_INVALID;
            fprintf(stderr, "Invalid number of arguments passed.");
        }
        else
        {
            unsetenv(varName);
            printf("Variable %s removed successfully.\n", varName);
        }
    }
    else if (strcomp(in, "bg"))
    {
        char *tmp1;
        char *token = strtok_r(in, " \n", &tmp1);
        token = strtok_r(NULL, " \n", &tmp1);
        if (token == NULL || strtok_r(NULL, " \n", &tmp1) != NULL)
        {
            p_exitID = ERR_INVALID;
            fprintf(stderr, "Invalid number of arguments passed.");
        }
        else
        {
            p_exitID = ERR_404;
            int count = 0;
            for (int x = 0; x < stacsize; x++)
            {
                if (stac[x] != -1)
                {
                    count++;
                }
                if (count == atoi(token))
                {
                    kill(stac[x], SIGCONT);
                    p_exitID = 0;
                    break;
                }
            }
        }
    }
    else if (strcomp(in, "kjob"))
    {
        char *tmp1;
        char *token = strtok_r(in, " \n", &tmp1);
        token = strtok_r(NULL, " \n", &tmp1);
        int count = 1;
        int pNo = -1;
        int statUS = 0;
        while (token != NULL)
        {
            count++;
            if (count == 2)
            {
                pNo = atoi(token);
            }
            if (count == 3)
            {
                statUS = atoi(token);
            }
            token = strtok_r(NULL, " \t\n", &tmp1);
        }
        printf("%d is PNO\n", pNo);
        if (count != 3)
        {
            p_exitID = ERR_INVALID;
            fprintf(stderr, "Invalid Number of arguments passed\n");
        }
        else
        {
            p_exitID = ERR_404;
            int jId = 0;
            for (int x = 0; x < stacsize; x++)
            {
                if (stac[x] != -1)
                {
                    jId++;
                }
                if (jId == pNo)
                {
                    p_exitID = 0;
                    fprintf(stderr, "Killing process with PID :%d \n", stac[x]);
                    if (kill(stac[x], statUS) < 0)
                    {
                        p_exitID = -1;
                        perror("kill(status)");
                    }
                }
            }
        }
    }

    else if (strcomp(in, "nightswatch"))
    {
        printf("%s\n", in);
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
            p_exitID = ERR_INVALID;
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
        {
            int t101 = runParellal(in);
            namestac[stacsize] = (char *)malloc(1000);
            if (in[strlen(in) - 1] == '\n')
            {
                in[strlen(in) - 1] = ' ';
            }
            strcpy(namestac[stacsize], in);
            stac2[stacsize] = t101;
            stac[stacsize++] = t101;
        }
        else
        {
            p_exitID = runSerial(in, &fPid);
        }
    }
}

// Correct working
void handleC(int sig)
{
    printf("fPid = %d\n", fPid);
    p_exitID = ERR_SIGINT;
    if (fPid > 0 && fPid != getpid())
    {
        kill(fPid, SIGINT);
        fprintf(stderr, "Keyboard Interrupt.\n");
        fflush(stderr);
    }
}

int main()
{

    // setup signal handlers for background processes
    signal(SIGCHLD, handler);
    signal(SIGINT, handleC);
    signal(SIGTSTP, handleZ);

    // Stores the base directory path
    relp = (char *)malloc(LIST_LEN);
    getcwd(relp, LIST_LEN);
    lastDir[0] = (char *)malloc(LIST_LEN);
    lastDir[1] = (char *)malloc(LIST_LEN);
    strcpy(lastDir[0], relp);
    strcpy(lastDir[1], relp);
    // Set absulute path for history command
    strcpy(TILDA, relp);
    strcat(TILDA, "/logfiles/history.log");

    // get details for prompt-printing

    char *st = (char *)malloc(2 * LIST_LEN);
    int num;
    int TMPIN = dup(STDIN_FILENO);
    int TMPOUT = dup(STDOUT_FILENO);
    // dup2(STDIN_FILENO, TMPIN);
    // dup2(STDOUT_FILENO, TMPOUT);

    getLog();
    fflush(stdout);
    char *username = (char *)malloc(LIST_LEN);
    getlogin_r(username, LIST_LEN);
    char *system_name = (char *)malloc(LIST_LEN);
    gethostname(system_name, LIST_LEN);
    int estat = 0;

    while (1)
    {
        /*
         path stores the current path relative to ~ as the relp path
         This is done as the assignment specified that the direcotory from which the terminal is called is the base directory 
        */

        String path = (String)calloc(LIST_LEN, 1);
        getcwd(path, LIST_LEN);
        // strlen(relp);
        if (strcomp(path, relp))
        {
            path[strlen(relp) - 1] = '~';
            path = &(path[strlen(relp) - 1]);
        }

        // Display prompt Line
        printf(":'%c<\033[1;36m%s@\033[1;32m%s:\033[1;33m%s\033[0m> ", (p_exitID == 0 ? ')' : '('), username, system_name, path);
        p_exitID = 0;
        // Input string declarations
        char *inall = (char *)calloc(LIST_LEN, 1); // All ';' seperated inputs

        // Check if Ctrl + D is pressed
        if (fgets(inall, LIST_LEN, stdin) == NULL)
        {
            closeShell();
        }
        // free(tmpip);
        char *in = (char *)calloc(LIST_LEN, 1); // Individual Atomic inputs
        char *tmp_r_strtok;
        in = strtok_r(inall, ";", &tmp_r_strtok);
        while (in != NULL)
        {
            // Append History of Commands
            append_log(in);
            int countPipes = 0;
            for (int x = 0; x < strlen(in); x++)
            {
                if (in[x] == '|')
                {
                    countPipes++;
                }
            }
            // printf("Pipes : %d", countPipes);
            int fd[2];
            char *nextTOK;
            char *tmpIn = strtok_r(in, "|", &nextTOK);
            in = trimString(in);
            while (countPipes >= 0 && tmpIn != NULL && strlen(tmpIn) >= 1)
            {
                // char *pcom = (char *)malloc(LIST_LEN);
                // sprintf(pcom, "COMMAND : %s\n", tmpIn);
                // write(TMPOUT, pcom, strlen(pcom));
                if (countPipes != 0)
                {
                    countPipes--;
                    if (pipe(fd) < 0)
                    {
                        perror("PIPE ERROR ");
                        break;
                    }
                    // Write on pipe the O/P of the present command.
                    dup2(fd[1], STDOUT_FILENO);
                }
                else
                {
                    countPipes--;
                    dup2(TMPOUT, STDOUT_FILENO);
                }

                // FILE REDIRECTION higher preference than PIPES
                int fx = -1;
                for (int x = 0; x < strlen(tmpIn); x++)
                {
                    if (tmpIn[x] == '>' && tmpIn[x + 1] != '>' && (x != 0 && tmpIn[x - 1] != '>'))
                    {
                        if (fx == -1)
                            fx = x;
                        int counter = 0;
                        char *ipfn = (char *)calloc(1000, 1);
                        for (int y = x + 1; y < strlen(tmpIn); y++)
                        {
                            if (tmpIn[y] != ' ')
                                ipfn[counter++] = tmpIn[y];
                            if (tmpIn[y] != ' ' && (tmpIn[y + 1] == '\0' || tmpIn[y + 1] == ' ' || tmpIn[y + 1] == '\t' || tmpIn[y + 1] == '\n'))
                            {
                                break;
                            }
                        }
                        ipfn = trimString(ipfn);
                        if (dup2(open(ipfn, O_RDWR | O_CREAT | O_TRUNC | __O_LARGEFILE, 0644), STDOUT_FILENO) == -1)
                        {
                            dup2(TMPOUT, STDOUT_FILENO);
                            printf("ERROR : Output file not found. Setting output to stdout\n");
                        }
                        free(ipfn);
                    }
                    else if (tmpIn[x] == '<')
                    {
                        if (fx == -1)
                            fx = x;
                        char *ipfn = (char *)calloc(1000, 1);
                        for (int y = x + 1; y < strlen(tmpIn); y++)
                        {
                            ipfn[y - x - 1] = tmpIn[y];
                            if (tmpIn[y] != ' ' && tmpIn[y + 1] == ' ')
                            {
                                break;
                            }
                        }
                        ipfn = trimString(ipfn);
                        write(TMPOUT, "\n", 1);
                        write(TMPOUT, ipfn, strlen(ipfn));
                        write(TMPOUT, "\n", 1);
                        ipfn = trimString(ipfn);
                        if (dup2(open(ipfn, O_RDWR | __O_LARGEFILE), STDIN_FILENO) == -1)
                        {
                            dup2(TMPIN, STDIN_FILENO);
                            fprintf(stderr, "Error in finding input file, setting input to stdio instead.\n");
                        }
                        free(ipfn);
                    }
                    else if (tmpIn[x] == '>' && tmpIn[x + 1] == '>')
                    {
                        if (fx == -1)
                            fx = x;
                        char *ipfn = (char *)calloc(1000, 1);
                        for (int y = x + 2; y < strlen(in); y++)
                        {
                            ipfn[y - x - 2] = tmpIn[y];
                            if (tmpIn[y] != ' ' && tmpIn[y + 1] == ' ')
                            {
                                break;
                            }
                        }
                        ipfn = trimString(ipfn);
                        if (dup2(open(ipfn, O_RDWR | O_APPEND | O_CREAT | __O_LARGEFILE, 0664), STDOUT_FILENO) == -1)
                        {
                            dup2(TMPOUT, STDOUT_FILENO);
                            printf("ERROR : output file not found. Setting output to stdout\n");
                        }
                        free(ipfn);
                    }
                }
                for (int x = fx; x < strlen(tmpIn); x++)
                {
                    tmpIn[x] = ' ';
                }
                tmpIn = trimString(tmpIn);
                execCommand(tmpIn, path);
                estat = errno;
                // write(TMPOUT, "here", 4);
                tmpIn = strtok_r(NULL, "|", &nextTOK);

                // The content written in the pipe will serve as the input to next command
                if (tmpIn != NULL)
                {
                    dup2(fd[0], STDIN_FILENO);
                }
                else
                {
                    dup2(TMPOUT, STDOUT_FILENO);
                }
                close(fd[0]);
                close(fd[1]);
            }

            dup2(TMPIN, STDIN_FILENO);
            dup2(TMPOUT, STDOUT_FILENO);
            fflush(stdout);
            fflush(stdin);
            // Restoration of IO After each command
            in = strtok_r(NULL, ";", &tmp_r_strtok); // finds the next instruction
            strcpy(lastDir[1], lastDir[0]);
            getcwd(lastDir[0], LIST_LEN);
        }
    }
}
