/*
    Do error handling.
    Check for large files and \0

    After several trials, optimal speed is found between 75 and 100 mb of block size
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#define _FILE_OFFSET_BITS 64
#define BUFFER_LENGTH (100 * 1024 * 1024)

void makedir()
{
    long int check;
    char *dirname = "Assignment";
    check = mkdir(dirname, 0705);
    chmod("Assignment", 0705);
}

void display(long double completed, long double file_size)
{
    if (completed > 100)
        file_size = 3e9;
    printf("\r");
    for (int xx_ = 0; xx_ < 100; xx_++)
    {
        if (xx_ < (100 * completed) / file_size)
        {
            printf("=");
        }
        else if (xx_ - 1 <= ((int)((100 * completed) / file_size)))
        {
            printf(">");
        }
        else
        {
            printf("_");
        }
    }
    printf("\t%2Lf %% completed", (100 * completed) / file_size);
}

int main(int argc, char **argv)
{
    char ch[100];
    int x = 100;
    x = sprintf(ch, "the number is : %d\n", x);
    write(1, ch, 20);
    x = sprintf(ch, "the number is : %d\n", x);
    write(1, ch, 20);
    return 0;
}