#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{

    printf("PARENT");

    int fl = fork();
    if (fl == -1)
    {
        printf("Error in forking");
    }
    else if (fl == 0)
    {
        printf("Hello from child");
        for (int x = 0; x < 10000; x++)
        {
            printf("aaaa");
        }
        printf("\n\n");
    }
    else
    {
        printf("Hello From parent\n");
        wait(NULL);
        printf("Hello, child is dead\n");
    }
}