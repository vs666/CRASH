#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main()
{
    char *str = (char *)malloc(1000);
    read(STDIN_FILENO, str, 1000);
    if (str[strlen(str) - 1] == '\n')
    {
        str[strlen(str) - 1] = '|';
    }
    char *tok = strtok(str, "|");
    printf("%s\n", tok);
    FILE *fr = fopen(tok, "r+");
    if (fr == NULL)
    {
        printf("NULL read file\n");
    }
    tok = strtok(NULL, "|");
    if (tok == NULL)
    {
        printf("ERR: Tok is NULL\n");
    }
    else
    {
        printf("%s\n", tok);
    }
    FILE *fw = fopen(tok, "w+");
    if (fw == NULL)
    {
        printf("WRITE FILE is NULL\n");
    }
    int ar[2];
    pipe(ar);
    size_t siz = 10;
    char str1[20];
    while (fread(str1, 21, 1, fr))
    {
        printf("Debug point\n");
        fflush(stdout);
        printf("Writing to pipe : %s\n", str1);
        write(ar[1], str1, 20);
        char *str2 = (char *)malloc(100);
        read(ar[0], str2, 20);
        printf("Reading from pipe : %s\n", str2);
        fprintf(fw, "%s", str2);
        free(str2);
    }
    fclose(fw);
    fclose(fr);
    close(ar[0]);
    close(ar[1]);
}