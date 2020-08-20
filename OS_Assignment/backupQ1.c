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

// #define BUFFER_LENGTH (100 * 1024 * 1024)
#define BUFFER_LENGTH (12)

char p_b[100];

void init()
{
    for (int x = 0; x < 100; x++)
    {
        p_b[x] = '_';
    }
}
void makedir()
{
    long int check;
    char *dirname = "Assignment";
    check = mkdir(dirname, 0705);
    chmod("Assignment", 0705);
}

void display(long double completed, long double file_size)
{
    char *st = (char *)malloc(1000);
    int num = 0;
    num = sprintf(st, "\r");
    write(1, st, num);
    int prc = ((100 * completed) / file_size);
    for (int x = prc; x >= 0; x--)
    {
        if (p_b[x] == '_' || p_b[x] == '>')
        {
            p_b[x] = '=';
        }
        else if (p_b == '=')
        {
            break;
        }
        if (x == prc)
        {
            p_b[x] = '>';
        }
    }

    num = sprintf(st, "%s\t%2Lf %% completed", p_b, ((100 * completed) / file_size));
    write(1, st, num);
}

int main(int argc, char **argv)
{
    char *st = (char *)malloc(1000);
    int num = 0;
    init();
    long long int fptr = open(argv[1], O_RDONLY | __O_LARGEFILE, 0600);
    makedir();
    char *final_path = (char *)malloc(1000);
    char dn[11] = "Assignment/";
    for (int x = 0; x < (sizeof(dn) / sizeof(char)); x++)
    {
        final_path[x] = dn[x];
    }

    char *chichi = (char *)malloc(1000);
    int india = 0;
    long int me = sizeof(argv[1]) / sizeof(char);
    for (int x = 1; x < me; x++)
    {
        if (argv[1][x - 1] == '/')
        {
            india++;
            chichi = &(argv[1][x]);
        }
    }
    if (india == 0)
        chichi = &argv[1][0];
    strcat(final_path, chichi);
    printf("file name :%s  %s\n", argv[1], final_path);
    long int ll = sizeof(dn) / sizeof(char);
    for (int x = 11; x <= 11 + (sizeof(argv[1]) / sizeof(char)); x++)
    {
        final_path[x] = argv[1][x - 11];
    }
    long long int optr = open(final_path, O_RDWR | O_TRUNC | __O_LARGEFILE | O_CREAT, 0600);
    long long int disp = BUFFER_LENGTH;

    if (fptr == -1 || optr == -1)
    {
        perror("UNABLE TO OPEN FILE. Error encountered.\nPlease make sure file exists and the filepath is correct\nError type");
    }
    else
    {
        num = sprintf(st, "Reversing and Copying file...\n\n");
        write(1, st, num);
        chmod(final_path, 0600);
        num = sprintf(st, "\e[?25l");
        write(1, st, num);
        long long int completed = 0;
        char cho[2];
        struct stat sta;
        stat(argv[1], &sta);
        long int file_size = sta.st_size;
        long int fff = file_size;

        long int ss = 0;
        long int br = 0;
        long int len = 0;
        long int c = 0;
        // long int r2 = lseek(fptr, -(BUFFER_LENGTH), SEEK_END);
        long long r2 = 0;
        char *din = (char *)malloc((sizeof(char) * BUFFER_LENGTH));
        char *stren = (char *)malloc((sizeof(char) * BUFFER_LENGTH));
        long long int rem_file_size = file_size;
        fff = file_size;
        while (r2 != -1 && rem_file_size >= BUFFER_LENGTH)
        {
            printf("read OP : %d\n", read(fptr, din, BUFFER_LENGTH));
            printf("din : %s\n", din);
            // printf("seek : %ld asked : %ld\n", lseek(fptr, (-2 * BUFFER_LENGTH), SEEK_CUR), rem_file_size - 2 * BUFFER_LENGTH);

            long int endIndex = BUFFER_LENGTH - 1;
            for (int x = 0; endIndex >= 0; x++, endIndex--)
            {
                stren[x] = din[endIndex];
                completed += 1;
                c++;
                if (c == 1024)
                {
                    c = 0;
                    display(completed, fff);
                }
            }
            rem_file_size -= BUFFER_LENGTH;
            write(optr, stren, BUFFER_LENGTH);
        }

        free(stren);
        free(din);
        if (rem_file_size > 0)
        {

            char *din = (char *)malloc((sizeof(char) * rem_file_size));
            char *stren = (char *)malloc((sizeof(char) * rem_file_size));
            file_size = rem_file_size;
            long int r1 = read(fptr, din, file_size);
            printf("\n\n%s is din \n\n", din);
            long int endIndex = file_size - 1;
            disp += file_size;
            long int tmp = file_size;
            file_size = 0;
            printf("\n\n");
            for (int x = 0; endIndex >= 0; x++, endIndex--)
            {
                stren[x] = din[endIndex];
                printf("%d ", din[endIndex]);
                completed += 1;
                c++;

                // if (c == 1024)
                // {
                //     display(completed, fff);
                //     c = 0;
                // }
            }
            printf("\n\nstren is : %s\n\n", stren);
            write(optr, stren, tmp);
            free(din);
            free(stren);
        }
        display(1, 1);
        num = sprintf(st, "\n~~~~~~~~Process complete~~~~~~~~\n");
        write(1, st, num);
        close(optr);
        close(fptr);
    }
    num = sprintf(st, "\e[?25h");
    write(1, st, num);
    return 0;
}