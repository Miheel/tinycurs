#include <iostream>
#include <termios.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <cmath>

#include "termmode.hpp"
#include "curs.hpp"
#include "keyboard.hpp"
#include "parse.hpp"

#include "stick_figure.hpp"

auto main() -> int
{
    setlocale(LC_ALL, "utf-8");
    signal(SIGWINCH, sigResize);
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
    std::cout << BpasteOn;
    echoOff();
    if (noBlock() != 0)
    {
        return 1;
    }
    std::string key = "No";

    Curs curs;
    curs.init();
    std::cout << curInvis;
    winsize winSize = curs.getwinsize(), newWinSize = curs.getwinsize();

    int width = 12, height = 6;
    auto x = (winSize.ws_col / 2) - ((width) / 2);
    int y = (winSize.ws_row / 2) - ((height) / 2);

    stick_figure stickF(x, y, width, height);
    stick_figure stick2f(0, 4, width, height);

    float relx = 0;
    float rely = 0;
    int new_x = 0;
    int new_y = 0;

    while (true)
    {

        pollKey();

        if (isKeyPressed(UP_KEY))
        {
            key = "Up";
            if (stickF.getPos_Size().topY >= 1)
                stickF.move(0, -1);
        }
        if (isKeyPressed(DOWN_KEY))
        {
            key = "Down";
            if ((stickF.getPos_Size().topY + stickF.getPos_Size().height) < winSize.ws_row)
                stickF.move(0, 1);
        }
        if (isKeyPressed(LEFT_KEY))
        {
            key = "Left";
            if ((stickF.getPos_Size().topX + stickF.getPos_Size().width) < winSize.ws_col)
                stickF.move(1, 0);
        }
        if (isKeyPressed(RIGHT_KEY))
        {
            key = "Right";
            if (stickF.getPos_Size().topX >= 1)
                stickF.move(-1, 0);
        }
        if (resized == true)
        {
            resized = false;
            curs.resize();
            newWinSize = curs.getwinsize();
            relx = (float)stickF.getPos_Size().topX / winSize.ws_col;
            rely = (float)stickF.getPos_Size().topY / winSize.ws_row;
            new_x = relx * newWinSize.ws_col;
            new_y = rely * newWinSize.ws_row;
            winSize = newWinSize;
            stickF.setpos(new_x, new_y);
        }

        if (isKeyPressed(ENTER_KEY))
        {
            resetAllStyle();
            clearscr();
            std::cout << curVis;
            std::cout << "Exiting\n";
            resetEcho();
            resetBlock();
            return 0;
        }

        curs.clear();
        curs.print(0, 0, key + " key was pressed");
        curs.print(1, 0, std::to_string(curs.getwinsize().ws_col) + ":" + std::to_string(curs.getwinsize().ws_row));
        curs.print(2, 0, std::to_string(stickF.getPos_Size().topX) + ":" + std::to_string(stickF.getPos_Size().topY));
        curs.print(3, 0, std::to_string(new_x) + ":" + std::to_string(new_y));
        stick2f.draw(curs);
        stickF.draw(curs);

        curs.endframe();
    }

    resetEcho();
    resetBlock();

    return 0;
}