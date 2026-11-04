#ifndef UTIL_HPP
#define UTIL_HPP

#include <iostream>
#include <string>
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

#endif
