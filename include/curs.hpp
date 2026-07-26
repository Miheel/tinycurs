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
    std::vector<Pane> panes;
    struct winsize _winsize;

public:
    Curs() {}
    Curs(int width, int height);

    void init();
    void initWinsize();
    winsize getwinsize();
    void addpane(int topX, int topY, int width, int height);
    void print(int row, int col, std::string str);
    void print(int pane, int row, int col, std::string str);
    void resize();
    void clear();
    void draw(Pane &pane);
    void endframe();
    ~Curs() {}
};

#endif
