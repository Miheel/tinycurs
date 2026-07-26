#ifndef PANE_HPP
#define PANE_HPP

#include <iostream>
#include <string>
#include <vector>
#include "esc_code.hpp"

enum color
{
    Black,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White,
    Default
};

struct Cell
{
    std::string glyph = " ";
    // pane acts as a paintbrush if bd is other than black.
    color bg = color::Black;
    color fg = color::White;
};

inline void emitChar(const std::string &glyph)
{
    std::cout << glyph;
}

inline void emitCell(const Cell &cell)
{
    resetAllStyle();
    emitColorID(cell.fg, cell.bg);
    emitChar(cell.glyph);
}

struct Pos_size
{
    int topX = 0;
    int topY = 0;
    int width = 0;
    int height = 0;
};

struct Pane
{
    Pane(int topX, int topY, int width, int height);

    void addBorder();

    void setTitle(std::string title);

    void setpos(int x, int y);

    Pos_size &getPos_Size();

    void print(int row, int col, std::string str);

    void printTextF(int row, int col, std::string str, color bg, color fg);

    std::vector<std::vector<Cell>> virtual_pane;
    std::string title{};
    Pos_size pos_size;
    std::string borderH = "│";
    std::string borderv = "─";

    std::string topLcorner = "┌";
    std::string topRcorner = "┐";
    std::string botLcorner = "└";
    std::string botRcorner = "┘";

    std::string topLcornerround = "╭";
    std::string topRcornerround = "╮";
    std::string botLcornerround = "╰";
    std::string botRcornerround = "╯";
};

#endif // PANE_HPP