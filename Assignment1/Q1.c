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

#define BUFFER_LENGTH (100 * 1024 * 1024L)
// #define BUFFER_LENGTH (12)

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
    unsigned long int check;
    char *dirname = "Assignment";
    check = mkdir(dirname, 0700);
    chmod("Assignment", 0700);
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
        else if (p_b[x] == '=')
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
    num = sprintf(st, "\n\n");
    write(1, st, num);
    init();

    if (argc != 2)
    {
        num = sprintf(st, "Incorrect Number of arguments passed.\n");
        write(1, st, num);
        return 0;
    }
    unsigned long int fptr = open(argv[1], O_RDONLY | __O_LARGEFILE);

    makedir();
    char *final_path = (char *)malloc(1000);
    char dn[11] = "Assignment/";
    for (int x = 0; x < 11; x++)
    {
        final_path[x] = dn[x];
    }
    char *chichi = (char *)malloc(1000);
    int india = 0;
    unsigned long int me = sizeof(argv[1]) / sizeof(char);
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

    unsigned long int optr = open(final_path, O_RDWR | O_TRUNC | __O_LARGEFILE | O_CREAT, 0600);
    unsigned long int disp = BUFFER_LENGTH;

    if (fptr == -1 || optr == -1)
    {
        perror("File Open error ");
    }
    else
    {
        num = sprintf(st, "Reversing and Copying file...\n\n");
        write(1, st, num);

        chmod(final_path, 0600);

        num = sprintf(st, "\e[?25l");
        write(1, st, num);
    
        unsigned long int completed = 0;

        struct stat sta;
        stat(argv[1], &sta);
        unsigned long int file_size = sta.st_size;
        unsigned long int fff = file_size;
        unsigned long int rem_file_size = file_size;

        unsigned long int c = 0;
        long long r2 = 0;

        (unsigned long int)lseek(fptr, 0, SEEK_END);
        r2 = (unsigned long int)lseek(fptr, (__off64_t)-BUFFER_LENGTH, SEEK_CUR);

        int a = 1;

        while (rem_file_size >= BUFFER_LENGTH)
        {
            char *din2 = (char *)malloc(BUFFER_LENGTH);
            char *stren = (char *)malloc(BUFFER_LENGTH);

            int rt = read(fptr, din2, BUFFER_LENGTH);

            long int endIndex = BUFFER_LENGTH - 1;
            for (unsigned long int x = 0; endIndex >= 0; x++)
            {
                stren[x] = din2[endIndex];
                completed += 1;
                c++;
                if (c == 1024)
                {
                    c = 0;
                    display(completed, fff);
                }
                endIndex--;
            }

            rem_file_size -= BUFFER_LENGTH;
            fflush(stdout);
            write(optr, stren, BUFFER_LENGTH);
            r2 = (long int)lseek(fptr, (__off64_t)(-2 * BUFFER_LENGTH), SEEK_CUR);

            free(din2);
            free(stren);
        }

        if (rem_file_size > 0)
        {

            char *din3 = (char *)malloc(rem_file_size);
            char *stren2 = (char *)malloc(rem_file_size);

            file_size = rem_file_size;
            lseek(fptr, 0, SEEK_SET);
            unsigned long int r1 = read(fptr, din3, rem_file_size);
            long int endIndex = rem_file_size - 1;
            disp += file_size;
            unsigned long int tmp = file_size;
            file_size = 0;

            for (int x = 0; endIndex >= 0; x++, endIndex--)
            {
                stren2[x] = din3[endIndex];
                completed += 1;
                c++;

                if (c == 1024)
                {
                    display(completed, fff);
                    c = 0;
                }
            }
            write(optr, stren2, tmp);
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