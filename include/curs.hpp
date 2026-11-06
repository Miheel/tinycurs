#ifndef CURS_HPP
#define CURS_HPP

#include <iostream>
#include <vector>
#include <sys/ioctl.h>
#include <signal.h>
#include "pane.hpp"
#include "esc_code.hpp"
#include "signalHandler.hpp"
#include "termmode.hpp"

class Curs
{
private:
    std::vector<std::vector<Cell>> virtual_screen;
    std::vector<std::vector<Cell>> phys_screen;
    bool isRuning = false;
    struct winsize _winsize;
    TermMode termMode;

public:
    Curs() {}
    Curs(int width, int height);

    void init();
    void deinit();
    void initWinsize();
    void initTerm();
    winsize getWinsize();
    void print(int row, int col, std::string str);
    void resize();
    bool is_runing();
    void exit();
    void clear();
    void draw(const Pane &pane);
    void endframe();
    ~Curs();
};

#endif
