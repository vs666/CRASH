#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

#define BUFFER_LENGTH (100 * 1024 * 1024)

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

int refi(FILE *fptr, FILE *optr)
{
    printf("\e[?25l");

    fflush(stdout);

    fseek(fptr, 0, SEEK_END);
    long int file_size1 = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);
    long int fcomp = 0;
    fseek(optr, 0, SEEK_END);
    long int file_size2 = ftell(optr);
    fseek(optr, 0, SEEK_SET);

    if (file_size1 != file_size2)
    {
        return 0;
    }
    else if (file_size1 <= BUFFER_LENGTH)
    {
        int z = 0;
        char *ss = (char *)malloc((sizeof(char) * BUFFER_LENGTH));
        char *sr = (char *)malloc((sizeof(char) * BUFFER_LENGTH));
        fread(ss, sizeof(char), file_size1, fptr);
        fread(sr, sizeof(char), file_size1, optr);
        for (int x = file_size1 - 1; x >= 0; x--)
        {
            z++;
            if (ss[x] != sr[file_size1 - 1 - x])
            {
                return 0;
            }
            fcomp++;
            if (z == 1024)
            {
                z = 0;
                display(fcomp, file_size1);
            }
        }
        return 1;
    }

    long int disp = -BUFFER_LENGTH;
    fseek(optr, disp, SEEK_END);
    char *ss = (char *)malloc((sizeof(char) * BUFFER_LENGTH));
    char *sr = (char *)malloc((sizeof(char) * BUFFER_LENGTH));
    printf("Err1\n");
    while (disp >= -1 * file_size1 && fread(ss, sizeof(char), BUFFER_LENGTH, fptr) && fread(sr, sizeof(char), BUFFER_LENGTH, optr))
    {
        int z = 0;
        disp -= BUFFER_LENGTH;
        for (int x = BUFFER_LENGTH - 1; x >= 0; x--)
        {
            z++;
            if (ss[x] != sr[BUFFER_LENGTH - 1 - x])
            {
                return 0;
            }
            fcomp++;
            if (z == 1024)
            {
                z = 0;
                display(fcomp, file_size1);
            }
        }
        int r = fseek(optr, disp, SEEK_END);
        if (r != 0)
        {
            break;
        }
    }

    char *ss1 = (char *)malloc((sizeof(char) * BUFFER_LENGTH));
    char *sr1 = (char *)malloc((sizeof(char) * BUFFER_LENGTH));
    int r = fseek(fptr, -BUFFER_LENGTH, SEEK_END);
    fseek(optr, 0L, SEEK_SET);
    if (r != 0)
    {
        fseek(fptr, 0L, SEEK_SET);
    }
    fread(ss1, sizeof(char), BUFFER_LENGTH, fptr);
    fread(sr1, sizeof(char), BUFFER_LENGTH, optr);
    if (sizeof(ss1) != sizeof(sr1))
    {
        return 0;
    }
    int z = 0;
    for (int x = (sizeof(ss1) / sizeof(char)) - 1; x >= 0; x--)
    {
        z++;
        if (ss1[x] != sr1[(sizeof(sr1) / sizeof(char)) - 1 - x])
        {
            return 0;
        }
        fcomp++;
        if (z == 1024)
        {
            z = 0;
            display(fcomp, file_size1);
        }
    }

    return 1;
}

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        fprintf(stderr, "Incorrect number of arguments passed\n");
        exit(2);
    }
    for (int x = 1; x <= 3; x++)
    {
        struct stat fs;
        int r_val = stat(argv[x], &fs);
        if (r_val != 0)
        {
            if (errno == EACCES)
            {
                // ACCESS DENIED
                fprintf(stderr, "Access to the file/directory is denied");
            }
            else if (errno == EFAULT)
            {
                // bad address
                fprintf(stderr, "Bad Address");
            }
            else if (errno == ENOENT)
            {
                // Path doesnot exist
                if (x == 3)
                {
                    printf("Directory Created : No\n");
                }
                else
                    fprintf(stderr, "Path doesnot exist : %s", argv[x]);
            }
            else
            {
                fprintf(stderr, "Something went wrong ");
            }
        }
        else
        {
            if (x == 3)
                printf("Directory Exists : Yes\n");
            printf("User   has  read    permission on %s : %s\n", argv[x], ((fs.st_mode & S_IRUSR) ? "Yes" : "No"));
            printf("User   has  write   permission on %s : %s\n", argv[x], ((fs.st_mode & S_IWUSR) ? "Yes" : "No"));
            printf("User   has  execute permission on %s : %s\n", argv[x], ((fs.st_mode & S_IXUSR) ? "Yes" : "No"));
            printf("Group  has  read    permission on %s : %s\n", argv[x], ((fs.st_mode & S_IRGRP) ? "Yes" : "No"));
            printf("Group  has  write   permission on %s : %s\n", argv[x], ((fs.st_mode & S_IWGRP) ? "Yes" : "No"));
            printf("Group  has  execute permission on %s : %s\n", argv[x], ((fs.st_mode & S_IXGRP) ? "Yes" : "No"));
            printf("Others have read    permission on %s : %s\n", argv[x], ((fs.st_mode & S_IROTH) ? "Yes" : "No"));
            printf("Others have write   permission on %s : %s\n", argv[x], ((fs.st_mode & S_IWOTH) ? "Yes" : "No"));
            printf("Others have execute permission on %s : %s\n", argv[x], ((fs.st_mode & S_IXOTH) ? "Yes" : "No"));
        }
        printf("\n-----------------------------------------------------------------\n\n");
    }
    FILE *fptr, *optr;
    fptr = fopen(argv[1], "r");
    optr = fopen(argv[2], "r");
    int rt = refi(fptr, optr);
    printf("\e[?25h");
    display(1, 1);
    printf("\n");
    if (rt == 1)
    {
        printf("Files %s and %s are reverse of Each other.\n", argv[1], argv[2]);
    }
    else
    {
        printf("Files %s and %s are not reverse of Each other.\n", argv[1], argv[2]);
    }
    return 0;
}