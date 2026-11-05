#ifndef TERMMODE_HPP
#define TERMMODE_HPP

#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include "esc_code.hpp"

struct termios old_termios, echo_off_termios;
int oldf, newf;
bool resized = false;
// handle terminal resize and inject RESIZE_key event into the event queue
void sigResize(int sig)
{
    resized = true;
}
void sighand(int signal)
{
    std::cout << signal << " raised\n";
    tcsetattr(fileno(stdin), TCSANOW, &old_termios);
    resetAllStyle();
    std::cout << curVis;
    exit(signal);
}

auto echoOff() -> int
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

auto resetEcho() -> int
{
    // old_termios.c_lflag |= (ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSANOW, &old_termios) == -1)
    {
        std::cerr << "Error resetting terminal mode\n";
        return -1;
    }
    return 0;
}

auto noBlock() -> int
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

auto resetBlock() -> int
{
    return fcntl(STDIN_FILENO, F_SETFL, oldf);
}

#endif
