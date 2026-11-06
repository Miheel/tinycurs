#include "termmode.hpp"

int TermMode::echoOff()
{
    // on success, tcgetattr returns 0. On error, returns -1 and sets errno.
    if (tcgetattr(STDIN_FILENO, &old_termios) == -1)
    {
        std::cerr << "Error getting terminal mode\n";
        return -1;
    }
    echo_off_termios = old_termios;
    echo_off_termios.c_lflag &= ~(ICANON | ECHO);
    //  cfmakeraw(&echo_off_termios);
    if (tcsetattr(STDIN_FILENO, TCSANOW, &echo_off_termios) == -1)
    {
        std::cerr << "Error setting terminal mode\n";
        return -2;
    }
    return 0;
}

int TermMode::resetEcho()
{
    // old_termios.c_lflag |= (ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSANOW, &old_termios) == -1)
    {
        std::cerr << "Error resetting terminal mode\n";
        return -1;
    }
    return 0;
}

int TermMode::noBlock()
{
    // on success, fcntl returns the file descriptor. On error, returns -1 and sets errno.
    oldf = fcntl(STDIN_FILENO, F_GETFL);
    if (oldf == -1)
    {
        std::cerr << "Error getting file status flags\n";
        return -1;
    }
    newf = oldf;

    return fcntl(STDIN_FILENO, F_SETFL, newf | O_NONBLOCK);
}

int TermMode::resetBlock()
{
    return fcntl(STDIN_FILENO, F_SETFL, oldf);
}

winsize TermMode::getWindowSize()
{
    return ::getWindowSize();
}

TermMode::~TermMode()
{
    resetEcho();
    resetBlock();
}

winsize getWindowSize()
{
    winsize ws;
    int status = ioctl(STDERR_FILENO, TIOCGWINSZ, &ws);
    if (status == -1)
    {
        std::cerr << "Error getting window size\n";
    }
    return ws;
}
