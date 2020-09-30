#include "includefiles.h"
int main(int argc, char *argv[])
{
    /* 
        Argument argv[0] = './a.out' 
        Argument argv[1] = 'command'
        Argument argv[2] = 'current path'
        Argument argv[3] = 'root path'
    */

    char *command = (char *)malloc(1000);
    command = argv[1];

    // if we are in root folder and the comand is cd ..
    if (strcmp(argv[2], argv[3]) && command[0] == '.' && command[1] == '.')
    {
        // display the current directory

        char *st = (char *)malloc(1000);
        int n = sprintf(st, "%s", argv[3]);
        write(1, st, n);

        // free up memory

        free(st);
        free(command);
        return 0;
    }

    char *ar[100];
    char *path[100];

    for (int x = 0; x < 100; x++)
    {
        path[x] = (char *)malloc(100);
        ar[x] = (char *)malloc(100);
    }
    char del[2] = "/";
    char *token = strtok(argv[2], del);
    char *token2 = strtok(argv[3], del);
    char *token3 = strtok(argv[1], del);

    while (token2 != NULL && strcmp(token, token2) == 0)
    {
        token = strtok(NULL, del);
        token2 = strtok(NULL, del);
    }

    

    // free all variables
}
