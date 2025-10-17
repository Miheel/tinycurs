#include <iostream>
#include <termios.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "termmode.hpp"
#include "curs.hpp"
#include "keyboard.hpp"

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

    echoOff();
    if (noBlock() != 0)
    {
        return 1;
    }

    while (true)
    {
        pollKey();

        if (isKeyPressed(UP_KEY))
        {
            std::cout << "Up key was pressed\n";
        }
        if (isKeyPressed(DOWN_KEY))
        {
            std::cout << "Down key was pressed\n";
        }
        if (isKeyPressed(LEFT_KEY))
        {
            std::cout << "Left key was pressed\n";
        }
        if (isKeyPressed(RIGHT_KEY))
        {
            std::cout << "Right key was pressed\n";
        }
        if (isKeyPressed(ENTER_KEY))
        {
            break;
        }
    }

    // try
    // {
    //     Curs curs(100, 20);
    //     curs.print(4, 10, "hello world");
    //     curs.print(4, 16, "goodbye");
    //     curs.update();
    //     Pane pane1(10, 2, 20, 10);
    //     pane1.addBorder();
    //     Pane pane2(30, 1, 30, 20);
    //     pane2.addBorder();
    //     pane1.print(5, 5, "hello pane");
    //     curs.update(pane1);
    //     for (size_t i = 0; i < 10; i++)
    //     {
    //         pane2.print(5, 5, "new pane" + std::to_string(i));
    //         curs.update(pane2);
    //         sleep(1);
    //     }
    // }
    // catch (const std::exception &e)
    // {
    //     std::cerr << e.what() << '\n';
    // }

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
    resetBlock();
    //    int i = 0;
    //    for (auto &e : rgb)
    //    {
    //        int r = e.r / 257;
    //        int g = e.g / 257;
    //        int b = e.b / 257;
    //        BGrgb(r, g, b);
    //        FGrgb(r, g, b);
    //        std::cout << "rgb";
    //        if (i == 7)
    //        {
    //            std::cout << "\n";
    //        }
    //        i++;
    //    }

    // std::cout << "\n"
    //           << Reset::all;

    return 0;
}