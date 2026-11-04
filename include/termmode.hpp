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
// handle terminal resize and inject RESIZE event into the event queue
void sigResize(int sig)
{
    resized = true;
    std::cout << RESIZE_KEY;
}
void sighand(int signal)
{
    std::cout << signal << " raised\n";
    tcsetattr(fileno(stdin), TCSANOW, &old_termios);
    resetAllStyle();
    std::cout << curVis;
    exit(signal);
}

void echoOff()
{
    int getter = tcgetattr(STDIN_FILENO, &old_termios), setter;
    echo_off_termios = old_termios;
    echo_off_termios.c_lflag &= ~(ICANON | ECHO);
    //  cfmakeraw(&echo_off_termios);
    setter = tcsetattr(STDIN_FILENO, TCSANOW, &echo_off_termios);
}

void resetEcho()
{
    // old_termios.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
}

auto noBlock() -> int
{
    oldf = fcntl(STDIN_FILENO, F_GETFL);
    newf = oldf;

    return fcntl(STDIN_FILENO, F_SETFL, newf | O_NONBLOCK);
}

auto resetBlock() -> int
{
    return fcntl(STDIN_FILENO, F_SETFL, oldf);
}

#endif
