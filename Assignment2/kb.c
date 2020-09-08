#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

char kbhit()
{
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(0, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO | ISIG);
    newt.c_cc[VMIN] = 1;
    newt.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF)
    {
        read(0, &ch, 1);
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return 0;
}
