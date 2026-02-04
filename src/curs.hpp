#ifndef CURS_HPP
#define CURS_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sys/ioctl.h>
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

void emitChar(const std::string &glyph)
{
    std::cout << glyph;
}

void emitCell(const Cell &cell)
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
    Pane(int topX, int topY, int width, int height)
        : virtual_pane(height, std::vector<Cell>(width))
    {
        pos_size.topX = topX,
        pos_size.topY = topY;
        pos_size.width = width;
        pos_size.height = height;
    }

    void addBorder()
    {
        for (size_t row = 0; row < this->pos_size.height; row++)
        {
            for (size_t col = 0; col < this->pos_size.width; col++)
            {
                if ((row == 0 || row == (this->pos_size.height - 1)) && (col > 0 && col < (this->pos_size.width - 1)))
                {
                    this->virtual_pane[row][col].glyph = borderv;
                    this->virtual_pane[row][col].bg = color::Blue;
                    this->virtual_pane[row][col].fg = color::White;
                }
                if ((col == 0 || col == (this->pos_size.width - 1)) && (row > 0 && row < (this->pos_size.height - 1)))
                {
                    this->virtual_pane[row][col].glyph = borderH;
                    this->virtual_pane[row][col].bg = color::Blue;
                    this->virtual_pane[row][col].fg = color::White;
                }
            }
        }

        virtual_pane[0][0].glyph = topLcornerround;                  //'+';                  //'┌';
        virtual_pane[0][pos_size.width - 1].glyph = topRcornerround; //'+'; //'┐';

        virtual_pane[pos_size.height - 1][0].glyph = botLcornerround;                  //'+';                  //'└';
        virtual_pane[pos_size.height - 1][pos_size.width - 1].glyph = botRcornerround; //'+'; //'┘';
    }

    void setTitle(std::string title)
    {
    }

    void setpos(int x, int y)
    {
        this->pos_size.topX = x;
        this->pos_size.topY = y;
    }

    Pos_size &getPos_Size()
    {
        return this->pos_size;
    }

    void print(int row, int col, std::string str)
    {
        for (size_t i = 0; i < str.size() && (col + 1) < this->pos_size.width; i++)
        {
            this->virtual_pane[row][col++].glyph = str[i];
        }
    }

    void printTextF(int row, int col, std::string str, color bg, color fg)
    {
        for (size_t i = 0; i < str.size() && (col + 1) < this->pos_size.width; i++)
        {
            this->virtual_pane[row][col].glyph = str[i];
            this->virtual_pane[row][col].bg = bg;
            this->virtual_pane[row][col++].fg = fg;
        }
    }

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
    void draw();
    void draw(Pane &pane);
    void endframe();
    ~Curs() {}
};

inline Curs::Curs(int width, int height)
{
    init();
    if (_winsize.ws_col < width || _winsize.ws_row < height)
    {
        std::cerr << "Window too small for app.\n";
        throw 1;
    }
}

inline void Curs::init()
{
    std::cout << Erase::all << CURSHOME;
    initWinsize();
    std::vector<std::vector<Cell>> tempbuff(_winsize.ws_row, std::vector<Cell>(_winsize.ws_col));
    virtual_screen = tempbuff;
    phys_screen = tempbuff;
}

inline void Curs::initWinsize()
{
    int status = ioctl(STDERR_FILENO, TIOCGWINSZ, &_winsize);
    if (status == -1)
    {
        std::cerr << "error";
    }
}

inline winsize Curs::getwinsize()
{
    return _winsize;
}

inline void Curs::addpane(int topX, int topY, int width, int height)
{
    Pane pane(topX, topY, width, height);

    panes.push_back(pane);
}

inline void Curs::print(int row, int col, std::string str)
{
    for (size_t i = 0; i < str.size() && (col + 1) < _winsize.ws_col; i++)
    {
        virtual_screen[row][col++].glyph = str[i];
    }
}

inline void Curs::print(int pane, int row, int col, std::string str)
{
    for (size_t i = 0; i < str.size() && (col + 1) < panes[pane].pos_size.width; i++)
    {
        panes[pane].virtual_pane[row][col++].glyph = str[i];
    }
}

inline void Curs::resize()
{
    init();
}

inline void Curs::clear()
{
    std::vector<std::vector<Cell>> tempbuff(_winsize.ws_row, std::vector<Cell>(_winsize.ws_col));
    virtual_screen = tempbuff;
}

inline void Curs::draw()
{
    for (auto &pane : panes)
    {
        int pr = 0;
        for (size_t row = pane.pos_size.topY; row < pane.pos_size.topY + pane.pos_size.height && pr < pane.pos_size.height; row++, pr++)
        {
            int pc = 0;
            for (size_t col = pane.pos_size.topX; col < pane.pos_size.topX + pane.pos_size.width && pc < pane.pos_size.width; col++, pc++)
            {
                if (virtual_screen[row][col].glyph != pane.virtual_pane[pr][pc].glyph)
                {
                    virtual_screen[row][col] = pane.virtual_pane[pr][pc];
                }
            }
        }
    }

    for (size_t row = 0; row < _winsize.ws_row; row++)
    {
        for (size_t col = 0; col < _winsize.ws_col; col++)
        {
            if (phys_screen[row][col].glyph != virtual_screen[row][col].glyph)
            {
                cursMove(row + 1, col + 1);
                emitCell(virtual_screen[row][col]);
                phys_screen[row][col] = virtual_screen[row][col];
            }
        }
    }
    std::cout.flush();
}

inline void Curs::draw(Pane &pane)
{
    int pr = 0;
    for (size_t row = pane.pos_size.topY; row < pane.pos_size.topY + pane.pos_size.height && pr < pane.pos_size.height; row++, pr++)
    {
        int pc = 0;
        for (size_t col = pane.pos_size.topX; col < pane.pos_size.topX + pane.pos_size.width && pc < pane.pos_size.width; col++, pc++)
        {
            if (virtual_screen[row][col].glyph != pane.virtual_pane[pr][pc].glyph ||
                (pane.virtual_pane[pr][pc].fg != virtual_screen[row][col].fg) ||
                (pane.virtual_pane[pr][pc].bg != virtual_screen[row][col].bg))
            {
                virtual_screen[row][col] = pane.virtual_pane[pr][pc];
            }
        }
    }
}

inline void Curs::endframe()
{
    for (size_t row = 0; row < _winsize.ws_row; row++)
    {
        for (size_t col = 0; col < _winsize.ws_col; col++)
        {
            if ((phys_screen[row][col].glyph != virtual_screen[row][col].glyph) ||
                (phys_screen[row][col].fg != virtual_screen[row][col].fg) ||
                (phys_screen[row][col].bg != virtual_screen[row][col].bg))
            {
                cursMove(row + 1, col + 1);
                emitCell(virtual_screen[row][col]);
                phys_screen[row][col] = virtual_screen[row][col];
            }
        }
    }

    std::cout.flush();
}

#endif
