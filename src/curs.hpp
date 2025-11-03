#ifndef CURS_HPP
#define CURS_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sys/ioctl.h>
#include "esc_code.hpp"

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
        : virtual_pane(height, std::string(width, ' '))
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
                    this->virtual_pane[row][col] = borderv;
                }
                if ((col == 0 || col == (this->pos_size.width - 1)) && (row > 0 && row < (this->pos_size.height - 1)))
                {
                    this->virtual_pane[row][col] = borderH;
                }
            }
        }

        virtual_pane[0][0] = '+';                  //'┌';
        virtual_pane[0][pos_size.width - 1] = '+'; //'┐';

        virtual_pane[pos_size.height - 1][0] = '+';                  //'└';
        virtual_pane[pos_size.height - 1][pos_size.width - 1] = '+'; //'┘';
    }

    void setTitle(std::string title)
    {
    }

    void print(int row, int col, std::string str)
    {
        for (size_t i = 0; i < str.size() & (col + 1) < this->pos_size.width; i++)
        {
            this->virtual_pane[row][col++] = str[i];
        }
    }

    std::vector<std::string> virtual_pane;
    std::string title{};
    Pos_size pos_size;
    unsigned char borderH = '|';
    unsigned char borderv = '-';
    std::string topLcorner = u8"┌";
    std::string topRcorner = u8"┐";
    std::string botLcorner = u8"└";
    std::string botRcorner = u8"┘";
};

class Curs
{
private:
    std::vector<std::string> virtual_screen;
    std::vector<std::string> phys_screen;
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
    void update();
    void update(Pane &pane);
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
    std::vector<std::string> tempbuff(_winsize.ws_row, std::string(_winsize.ws_col, ' '));
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
    for (size_t i = 0; i < str.size() & col + 1 < _winsize.ws_col; i++)
    {
        virtual_screen[row][col++] = str[i];
    }
}

inline void Curs::print(int pane, int row, int col, std::string str)
{
    for (size_t i = 0; i < str.size() & (col + 1) < panes[pane].pos_size.width; i++)
    {
        panes[pane].virtual_pane[row][col++] = str[i];
    }
}

inline void Curs::resize()
{
    init();
}

inline void Curs::clear()
{
    std::vector<std::string> tempbuff(_winsize.ws_row, std::string(_winsize.ws_col, ' '));
    virtual_screen = tempbuff;
}

inline void Curs::update()
{
    for (size_t i = 0; i < panes.size(); i++)
    {
        int pr = 0;
        for (size_t row = panes[i].pos_size.topY; row < panes[i].pos_size.topY + panes[i].pos_size.height && pr < panes[i].pos_size.height; row++, pr++)
        {
            int pc = 0;
            for (size_t col = panes[i].pos_size.topX; col < panes[i].pos_size.topX + panes[i].pos_size.width && pc < panes[i].pos_size.width; col++, pc++)
            {
                if (virtual_screen[row][col] != panes[i].virtual_pane[pr][pc])
                {
                    cursMove(row, col);
                    virtual_screen[row][col] = panes[i].virtual_pane[pr][pc];
                }
            }
        }
    }

    for (size_t i = 0; i < _winsize.ws_row; i++)
    {
        for (size_t j = 0; j < _winsize.ws_col; j++)
        {
            if (phys_screen[i][j] != virtual_screen[i][j])
            {
                cursMove(i, j);
                std::cout << virtual_screen[i][j];
                phys_screen[i][j] = virtual_screen[i][j];
            }
        }
    }
    std::cout.flush();
}

inline void Curs::update(Pane &pane)
{
    int pr = 0;
    for (size_t row = pane.pos_size.topY; row < pane.pos_size.topY + pane.pos_size.height && pr < pane.pos_size.height; row++, pr++)
    {
        int pc = 0;
        for (size_t col = pane.pos_size.topX; col < pane.pos_size.topX + pane.pos_size.width && pc < pane.pos_size.width; col++, pc++)
        {
            if (virtual_screen[row][col] != pane.virtual_pane[pr][pc])
            {
                cursMove(row, col);
                virtual_screen[row][col] = pane.virtual_pane[pr][pc];
            }
        }
    }

    for (size_t i = 0; i < _winsize.ws_row; i++)
    {
        for (size_t j = 0; j < _winsize.ws_col; j++)
        {
            if (phys_screen[i][j] != virtual_screen[i][j])
            {
                cursMove(i, j);
                std::cout << virtual_screen[i][j];
                phys_screen[i][j] = virtual_screen[i][j];
            }
        }
    }
    std::cout.flush();
}

#endif
