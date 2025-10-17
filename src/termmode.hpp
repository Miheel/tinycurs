#ifndef TERMMODE_HPP
#define TERMMODE_HPP

#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

struct termios old_termios, echo_off_termios;
int oldf, newf;

void sighand(int signal)
{
    std::cout << signal << " raised\n";
    tcsetattr(fileno(stdin), TCSANOW, &old_termios);
    exit(signal);
}

void echoOff()
{
    tcgetattr(STDIN_FILENO, &old_termios);
    echo_off_termios = old_termios;
    echo_off_termios.c_lflag &= ~(ICANON | ECHO);
    //  cfmakeraw(&echo_off_termios);
    tcsetattr(STDIN_FILENO, TCSANOW, &echo_off_termios);
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