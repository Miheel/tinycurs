#ifndef TERMMODE_HPP
#define TERMMODE_HPP

#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "esc_code.hpp"

class TermMode
{
private:
    struct termios old_termios, echo_off_termios;
    int oldf, newf;

public:
    TermMode() = default;

    int echoOff();

    int resetEcho();

    int noBlock();

    int resetBlock();

    winsize getWindowSize();

    ~TermMode();
};

winsize getWindowSize();
#endif
