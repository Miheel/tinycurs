#ifndef CURS_HPP
#define CURS_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sys/ioctl.h>
#include <unistd.h>
#include "pane.hpp"

class Curs
{
private:
    std::vector<std::vector<Cell>> virtual_screen;
    std::vector<std::vector<Cell>> phys_screen;
    bool isRuning = false;
    struct winsize _winsize;

public:
    Curs() {}
    Curs(int width, int height);

    void init();
    void initWinsize();
    winsize getwinsize();
    void print(int row, int col, std::string str);
    void resize();
    bool is_runing();
    void exit();
    void clear();
    void draw(const Pane &pane);
    void endframe();
    ~Curs() {}
};

#endif
