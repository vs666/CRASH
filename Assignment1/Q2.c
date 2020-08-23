#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

#define BUFFER_LENGTH (100 * 1024 * 1024L)

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

int refi(long int fptr, long int optr, long int file_size1, long int file_size2)
{
    int num;
    char *st = (char *)malloc(1000);
    num = sprintf(st, "\e[?25l");
    write(1, st, num);
    
    num = sprintf(st, "\n\n");
    write(1, st, num);
    
    lseek(fptr, 0, SEEK_SET);
    long int fcomp = 0;
    lseek(optr, 0, SEEK_SET);

    long int fff = file_size1;
    long int rem_file_size = file_size1;

    if (file_size1 != file_size2)
    {
        return 0;
    }
    else if (file_size1 <= BUFFER_LENGTH)
    {
        int z = 0;
        char *ss = (char *)malloc((sizeof(char) * file_size1));
        char *sr = (char *)malloc((sizeof(char) * file_size2));

        int err = read(fptr, ss, file_size1);
        if (err == -1)
        {
            perror("Error in reading file");
            return -1;
        }
        err = read(optr, sr, file_size2);
        if (err == -1)
        {
            perror("Error in reading file");
            return -1;
        }

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
        free(ss);
        free(sr);
        return 1;
    }

    long int disp = -BUFFER_LENGTH;
    lseek(optr, 0L, SEEK_END);
    lseek(optr, (__off64_t)(-1 * BUFFER_LENGTH), SEEK_CUR);
    int e1 = 1, e2 = 1;
    int count = 0;
    while (fff >= BUFFER_LENGTH && e1 && e2)
    {
        char *ss = (char *)malloc((sizeof(char) * BUFFER_LENGTH));
        char *sr = (char *)malloc((sizeof(char) * BUFFER_LENGTH));
        count++;
        e1 = read(fptr, ss, BUFFER_LENGTH);
        if (e1 == -1)
        {
            perror("File read error");
            return -1;
        }
        e2 = read(optr, sr, BUFFER_LENGTH);
        if (e2 == -1)
        {
            perror("File read error");
            return -1;
        }
        fff -= BUFFER_LENGTH;
        int z = 0;
        for (long int x = BUFFER_LENGTH - 1; x >= 0; x--)
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
        int r = lseek(optr, (__off64_t)(-2 * BUFFER_LENGTH), SEEK_CUR);
        free(ss);
        free(sr);
    }

    char *ss1 = (char *)malloc((sizeof(char) * BUFFER_LENGTH));
    char *sr1 = (char *)malloc((sizeof(char) * BUFFER_LENGTH));
    int r = lseek(fptr, -BUFFER_LENGTH, SEEK_END);
    lseek(optr, 0L, SEEK_SET);
    e1 = read(fptr, ss1, BUFFER_LENGTH);
    e2 = read(optr, sr1, BUFFER_LENGTH);
    if (e1 == -1 || e2 == -1)
    {
        perror("File read error");
        return -1;
    }
    if (sizeof(ss1) != sizeof(sr1))
    {
        return 0;
    }
    int z = 0;
    for (int x = BUFFER_LENGTH - 1; fff != 0 && x >= 0; x--)
    {
        z++;
        if (ss1[x] != sr1[BUFFER_LENGTH - 1 - x])
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
    int num;
    init();
    char *st = (char *)malloc(1000);
    if (argc != 4)
    {
        errno = EINVAL;
        perror("Incomplete number of arguments passed. Error");
        return 2;
    }
    for (int x = 1; x <= 3; x++)
    {
        struct stat fs;
        int r_val = stat(argv[x], &fs);
        if (r_val != 0)
        {
            // Path doesnot exist
            if (x == 3 && errno == ENONET)
            {
                num = sprintf(st, "Directory Created : No\n");
                write(1, st, num);
            }
            else
                perror("stat command error : ");
        }
        else
        {
            if (x == 3)
                num = sprintf(st, "Directory Exists : Yes\n");
            write(1, st, num);
            num = sprintf(st, "User   has  read    permission on %s : %s\n", argv[x], ((fs.st_mode & S_IRUSR) ? "Yes" : "No"));
            write(1, st, num);
            num = sprintf(st, "User   has  write   permission on %s : %s\n", argv[x], ((fs.st_mode & S_IWUSR) ? "Yes" : "No"));
            write(1, st, num);
            num = sprintf(st, "User   has  execute permission on %s : %s\n", argv[x], ((fs.st_mode & S_IXUSR) ? "Yes" : "No"));
            write(1, st, num);
            num = sprintf(st, "Group  has  read    permission on %s : %s\n", argv[x], ((fs.st_mode & S_IRGRP) ? "Yes" : "No"));
            write(1, st, num);
            num = sprintf(st, "Group  has  write   permission on %s : %s\n", argv[x], ((fs.st_mode & S_IWGRP) ? "Yes" : "No"));
            write(1, st, num);
            num = sprintf(st, "Group  has  execute permission on %s : %s\n", argv[x], ((fs.st_mode & S_IXGRP) ? "Yes" : "No"));
            write(1, st, num);
            num = sprintf(st, "Others have read    permission on %s : %s\n", argv[x], ((fs.st_mode & S_IROTH) ? "Yes" : "No"));
            write(1, st, num);
            num = sprintf(st, "Others have write   permission on %s : %s\n", argv[x], ((fs.st_mode & S_IWOTH) ? "Yes" : "No"));
            write(1, st, num);
            num = sprintf(st, "Others have execute permission on %s : %s\n", argv[x], ((fs.st_mode & S_IXOTH) ? "Yes" : "No"));
            write(1, st, num);
        }
        num = sprintf(st, "\n-----------------------------------------------------------------\n\n");
        write(1, st, num);
    }

    long int fptr, optr;
    struct stat sta1;
    stat(argv[1], &sta1);
    long int file_size1 = sta1.st_size;
    struct stat sta2;
    stat(argv[2], &sta2);
    long int file_size2 = sta2.st_size;

    fptr = open(argv[1], O_RDONLY | __O_LARGEFILE, S_IRUSR);
    if (fptr == -1)
    {
        perror("Error in opening file :");
        return 0;
    }
    optr = open(argv[2], O_RDONLY | __O_LARGEFILE, S_IRUSR);
    if (optr == -1)
    {
        perror("Error in opening file :");
        return 0;
    }
    int rt = refi(fptr, optr, file_size1, file_size2);
    num = sprintf(st, "\e[?25h");
    write(1, st, num);
    if (rt == -1)
    {
        num = sprintf(st, "\r");
        write(1, st, num);
        return 2;
    }
    display(1, 1);
    num = sprintf(st, "\n");
    write(1, st, num);
    if (rt == 1)
    {
        num = sprintf(st, "Files %s and %s are reverse of Each other.\n", argv[1], argv[2]);
        write(1, st, num);
    }
    else
    {
        num = sprintf(st, "Files %s and %s are not reverse of Each other.\n", argv[1], argv[2]);
        write(1, st, num);
    }
    return 0;
}