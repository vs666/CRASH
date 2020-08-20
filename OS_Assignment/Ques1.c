#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <math.h>

#define BUFFER_LENGTH (1024 * 1024 * 100)

void makedir()
{
    int check;
    char *dirname = "Assignment";
    check = mkdir(dirname, 0705);
}

void display(long double completed, long double file_size)
{
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
    FILE *fptr, *optr, *t__fptr;
    char *din = (char *)malloc((sizeof(char) * BUFFER_LENGTH));
    makedir();
    fptr = fopen(argv[1], "r");
    t__fptr = fopen(argv[1], "r");
    char *final_path = (char *)malloc(1000);
    char dn[11] = "Assignment/";
    for (int x = 0; x < (sizeof(dn) / sizeof(char)); x++)
    {
        final_path[x] = dn[x];
    }
    int me = sizeof(argv[1]) / sizeof(char);
    for (int x = 0; x < me; x++)
    {
        if (argv[1][x - 1] == '/')
            argv[1] = &(argv[1][x]);
    }
    int ll = sizeof(dn) / sizeof(char);
    for (int x = 11; x <= 11 + (sizeof(argv[1]) / sizeof(char)); x++)
    {
        final_path[x] = argv[1][x - 11];
    }
    optr = fopen(final_path, "a");

    long int disp = BUFFER_LENGTH;
    if (fptr == NULL || optr == NULL)
    {
        perror("UNABLE TO OPEN FILE. Error encountered.\nPlease make sure file exists and the filepath is correct\nError type");
    }
    else
    {
        chmod(final_path, 0600);
        printf("\e[?25l");
        long int percentage = 0;
        long int completed = 0;
        fseek(fptr, 0, SEEK_END);
        long int file_size = ftell(fptr);
        long int ss = 0;
        int br = 0;
        char *__tt = (char *)malloc(sizeof(char) * BUFFER_LENGTH);
        int ret = fseek(fptr, -disp, SEEK_END);
        int len = 0;
        int c = 0;
        while (br != 1 && ret == 0 && (fread(__tt, sizeof(char), BUFFER_LENGTH, t__fptr)) != 0)
        {
            fread(din, sizeof(char), BUFFER_LENGTH, fptr);
            disp += BUFFER_LENGTH;
            int fs = fseek(fptr, -disp, SEEK_END);
            ss = ftell(fptr);
            if (fs != 0)
            {
                fseek(fptr, 0L, SEEK_SET);
                br = 1;
            }
            len = sizeof(din);
            char *stren = (char *)malloc((sizeof(char) * BUFFER_LENGTH));

            int endIndex = 0;
            while (endIndex != BUFFER_LENGTH && din[endIndex] != '\0')
            {
                endIndex++;
            }
            endIndex--;
            for (int x = 0; endIndex >= 0; x++, endIndex--)
            {
                stren[x] = din[endIndex];
                completed = completed + 1;
                c++;
                if (c == 1024)
                {
                    c = 0;
                    display(completed, file_size);
                }
            }
            free(din);
            din = (char *)malloc((sizeof(char) * BUFFER_LENGTH));
            fprintf(optr, "%s", stren);

            //  print completed file
            display(completed, file_size);
        }
        completed = file_size;
        display(completed, file_size);
        display(1, 1);
        printf("\nFile conversion complete.\n");
        ss -= BUFFER_LENGTH;
        if (ret != 0)
        {
            fseek(fptr, 0, SEEK_END);
            ss = ftell(fptr);
        }
        if (ss != 0)
        {
            fseek(fptr, 0L, SEEK_SET);
            fread(din, sizeof(char), ss, fptr);
            char *stren = (char *)malloc((sizeof(char) * BUFFER_LENGTH));
            for (int x = ss - 1; x >= 0; x--)
            {
                stren[ss - 1 - x] = din[x];
            }
            fprintf(optr, "%s", stren);
        }
        fclose(fptr);
        fclose(optr);
    }
    printf("\e[?25h");
    return 0;
}