#ifndef PANE_HPP
#define PANE_HPP

#include <iostream>
#include <string>
#include <vector>
#include "util.hpp"
#include "element.hpp"

struct Pane
{
    Pane(int topX, int topY, int width, int height);
    void setpos(int x, int y);
    Pos_size &getPos_Size();
    std::vector<std::vector<Cell>> &getVirtualPane();

    void print(int row, int col, std::string str);
    void printTextF(int row, int col, std::string str, color bg, color fg);

    void clear();
    void render();

    std::vector<std::vector<Cell>> virtual_pane;
    std::vector<Element *> elements;
    std::vector<Pane *> child_panes;
    Pos_size pos_size;
};

#endif // PANE_HPP
