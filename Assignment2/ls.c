#include "ls.h"
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <pwd.h>

typedef char *String;

void ls_main(String s, String path)
{
    int HIDDEN = 0;
    int LISTALL = 0;
    for (int x = 0; x < strlen(s); x++)
    {
        if (s[x] == 'l')
        {
            LISTALL = 1;
        }
        else if (s[x] == 'a')
        {
            HIDDEN = 1;
        }
    }

    DIR *dir = opendir(path);
    if (dir == NULL)
    {
        printf("\033[1;31m Error : Directory %s doesnot exist.\033[0m\n", path);
        return;
    }
    if (LISTALL)
    {
        // Tabular listing
        struct dirent *dd = readdir(dir);
        while (dd != NULL)
        {
            if (dd->d_name[0] != '.' || HIDDEN == 1)
            {
                char s1[] = "----------\0";
                if (dd->d_type == DT_DIR)
                {
                    s1[0] = 'd';
                }
                struct stat st;
                String path1 = (String)malloc(1000);
                strcpy(path1, path);
                strcat(path1, "/");
                strcat(path1, dd->d_name);
                stat(path1, &st);
                free(path1);
                if ((st.st_mode & S_IRUSR) == S_IRUSR)
                {
                    s1[1] = 'r';
                }
                if ((st.st_mode & S_IWUSR) == S_IWUSR)
                {
                    s1[2] = 'w';
                }
                if ((st.st_mode & S_IXUSR) == S_IXUSR)
                {
                    s1[3] = 'x';
                }
                if ((st.st_mode & S_IRGRP) == S_IRGRP)
                {
                    s1[4] = 'r';
                }
                if ((st.st_mode & S_IWGRP) == S_IWGRP)
                {
                    s1[5] = 'w';
                }
                if ((st.st_mode & S_IXGRP) == S_IXGRP)
                {
                    s1[6] = 'x';
                }
                if ((st.st_mode & S_IROTH) == S_IROTH)
                {
                    s1[7] = 'r';
                }
                if ((st.st_mode & S_IWOTH) == S_IWOTH)
                {
                    s1[8] = 'w';
                }
                if ((st.st_mode & S_IXOTH) == S_IXOTH)
                {
                    s1[9] = 'x';
                }

                // printing in the long format
                struct passwd *p = getpwuid(st.st_uid);
                struct passwd *q = getpwuid(st.st_gid);
                time(0);
                time_t t1 = st.st_mtim.tv_sec;
                String stime = asctime(localtime(&t1));
                for (int x = strlen(stime) - 1; x >= 0; x--)
                {
                    if (stime[x] == '\n')
                    {
                        stime[x] = ' ';
                    }
                }
                printf("%s %ld %s %s %ld\t%s ", s1, st.st_nlink, p->pw_name, q->pw_name, st.st_size, stime);

                // color coding the mem-location type
                if ((st.st_mode & S_IXOTH) == S_IXOTH)
                {
                    printf("\033[1;32m");
                }
                if (dd->d_type == DT_DIR)
                {
                    printf("\033[1;34m");
                }
                printf("%s\n", dd->d_name);
                // stat(dd->d_name, &st);
                printf("\033[0m");
            }
            dd = readdir(dir);
        }
    }
    else
    {
        struct dirent *dd = readdir(dir);
        while (dd != NULL)
        {
            if (dd->d_name[0] != '.' || HIDDEN == 1)
            {
                struct stat st1;
                int st_rv = stat(dd->d_name, &st1);
                if ((st1.st_mode & S_IXOTH) == S_IXOTH)
                {
                    printf("\033[1;32m");
                }
                if (dd->d_type == DT_DIR)
                {
                    printf("\033[1;34m");
                }

                printf("%s   ", dd->d_name);
                printf("\033[0m");
            }
            dd = readdir(dir);
        }
        // Normal listing
    }
    printf("\n");
    closedir(dir);
}
