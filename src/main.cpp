#include <iostream>
#include <termios.h>
#include <signal.h>
#include <stdint.h>
#include "curs.hpp"

auto main() -> int
{
    signal(SIGABRT, sighand);
    signal(SIGFPE, sighand);
    signal(SIGILL, sighand);
    signal(SIGINT, sighand);
    signal(SIGSEGV, sighand);
    signal(SIGTERM, sighand);
    signal(SIGKILL, sighand);
    signal(SIGQUIT, sighand);
    signal(SIGCHLD, sighand);
    signal(SIGSTOP, sighand);
    signal(SIGSYS, sighand);

    // echoOff();
    Curs curs;
    curs.init();
    curs.print(4, 10, "hello world");
    curs.update();
    curs.print(4, 16, "goodbye");
    curs.update();

    curs.addpane("newpane", 10, 2, 20, 10);
    curs.paneBox(0);
    curs.print(0, 5, 5, "hello pane");
    curs.update();

    //     auto pos = curpos();
    //     std::cout << CURHOME;

    // for (size_t i = 0; i < 255; i++)
    // {
    //     FBcolorID(i);
    //     std::cout << i << " ";
    // }
    // std::cout << "\n";
    // std::cout << saveScr << curSavePos;

    // std::cout << CURHOME << "hello world" << std::flush;

    // std::cout << restoreScr << std::flush;

    // auto rgb = colorPalet16();

    resetEcho();

    // int i = 0;
    // for (auto &e : rgb)
    // {
    //     int r = e.r / 257;
    //     int g = e.g / 257;
    //     int b = e.b / 257;
    //     BGrgb(r, g, b);
    //     FGrgb(r, g, b);
    //     std::cout << "rgb";
    //     if (i == 7)
    //     {
    //         std::cout << "\n";
    //     }
    //     i++;
    // }

    // std::cout << "\n"
    //           << Reset::all;

    return 0;
}