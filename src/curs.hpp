#ifndef CURS_HPP
#define CURS_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sys/ioctl.h>
#include "esc_code.hpp"

struct Pane
{
    Pane(int topX, int topY, int width, int height)
        : topX(topX), topY(topY), width(width), height(height), virtual_pane(height, std::string(width, ' '))
    {
    }

    void addBorder()
    {
        for (size_t row = 0; row < this->height; row++)
        {
            for (size_t col = 0; col < this->width; col++)
            {
                if ((row == 0 || row == (this->height - 1)) && (col > 0 && col < (this->width - 1)))
                {
                    this->virtual_pane[row][col] = '-';
                }
                if ((col == 0 || col == (this->width - 1)) && (row > 0 && row < (this->height - 1)))
                {
                    this->virtual_pane[row][col] = '|';
                }

                // panes[pane].virtual_pane[0][0] = '┌';
                // panes[pane].virtual_pane[0][panes[pane].width] = '┐';

                // panes[pane].virtual_pane[panes[pane].height][0] = '└';
                // panes[pane].virtual_pane[panes[pane].height][panes[pane].width] = '┘';
            }
        }
    }

    void setTitle(std::string title)
    {
    }

    void print(int row, int col, std::string str)
    {
        for (size_t i = 0; i < str.size() & (col + 1) < this->width; i++)
        {
            this->virtual_pane[row][col++] = str[i];
        }
    }

    std::vector<std::string> virtual_pane;
    std::string title{};
    int topX = 0;
    int topY = 0;
    int width = 0;
    int height = 0;
    // char borderH = '|';
    // char borderv = '-';
    // char topLcorner = '┌';
    // char topRcorner = '┐';
    // char botLcorner = '└';
    // char botRcorner = '┘';
};

class Curs
{
private:
    std::vector<std::string> virtual_screen;
    std::vector<std::string> phys_screen;
    std::vector<Pane> panes;
    struct winsize _winsize;

public:
    Curs(int width, int height);

    void init();
    winsize getwinsize();
    void addpane(int topX, int topY, int width, int height);
    void print(int row, int col, std::string str);
    void print(int pane, int row, int col, std::string str);
    void update();
    void update(Pane &pane);
    ~Curs();
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
    getwinsize();
    std::vector<std::string> tempbuff(_winsize.ws_row, std::string(_winsize.ws_col, ' '));
    virtual_screen = tempbuff;
    phys_screen = tempbuff;
}

inline winsize Curs::getwinsize()
{
    int status = ioctl(STDERR_FILENO, TIOCGWINSZ, &_winsize);
    if (status == -1)
    {
        std::cerr << "error";
    }
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
    for (size_t i = 0; i < str.size() & (col + 1) < panes[pane].width; i++)
    {
        panes[pane].virtual_pane[row][col++] = str[i];
    }
}

inline void Curs::update()
{
    for (size_t i = 0; i < panes.size(); i++)
    {
        int pr = 0;
        for (size_t row = panes[i].topY; row < panes[i].topY + panes[i].height && pr < panes[i].height; row++, pr++)
        {
            int pc = 0;
            for (size_t col = panes[i].topX; col < panes[i].topX + panes[i].width && pc < panes[i].width; col++, pc++)
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
    for (size_t row = pane.topY; row < pane.topY + pane.height && pr < pane.height; row++, pr++)
    {
        int pc = 0;
        for (size_t col = pane.topX; col < pane.topX + pane.width && pc < pane.width; col++, pc++)
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

inline Curs::~Curs()
{
}

#endif
