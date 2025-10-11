#ifndef CURS_HPP
#define CURS_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sys/ioctl.h>
#include <unistd.h>

#include "esc_code.hpp"

struct termios old_termios, echo_off_termios;
void sighand(int signal)
{
    std::cout << signal << " raised\n";
    tcsetattr(fileno(stdin), TCSANOW, &old_termios);
    exit(signal);
}

void echoOff()
{
    tcgetattr(0, &old_termios);
    echo_off_termios = old_termios;
    echo_off_termios.c_lflag &= ~(ICANON | ECHO);
    //  cfmakeraw(&echo_off_termios);
    tcsetattr(fileno(stdin), TCSANOW, &echo_off_termios);
}

void resetEcho()
{
	// old_termios.c_lflag |= (ICANON | ECHO);
    tcsetattr(fileno(stdin), TCSANOW, &old_termios);
}

struct Pane
{
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
    std::vector<std::string> virtual_pane;
};

class Curs
{
private:
    std::vector<std::string> virtual_screen;
    std::vector<std::string> phys_screen;
    std::vector<Pane> panes;
    struct winsize _winsize;

public:
    Curs(/* args */);

    void init();
    winsize getwinsize();
    void addpane(std::string title = " ", int topX = 0, int topY = 0, int botX = 0, int botY = 0);
    void paneBox(int pane);
    void print(int row, int col, std::string str);
    void print(int pane, int row, int col, std::string str);
    void update();
    ~Curs();
};

Curs::Curs()
{
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
        std::cout << "error";
    }
    return _winsize;
}

inline void Curs::addpane(std::string title, int topX, int topY, int width, int height)
{
    Pane pane;
    pane.title = title;
    pane.topX = topX;
    pane.topY = topY;
    pane.width = width;
    pane.height = height;
    std::vector<std::string> tempbuff(height, std::string(width, ' '));
    pane.virtual_pane = tempbuff;
    panes.push_back(pane);
}

inline void Curs::paneBox(int pane)
{
    for (size_t row = 0; row < panes[pane].height; row++)
    {
        for (size_t col = 0; col < panes[pane].width; col++)
        {
            if ((row == 0 || row == (panes[pane].height - 1)) && (col > 0 && col < (panes[pane].width - 1)))
            {
                panes[pane].virtual_pane[row][col] = '-';
            }
            if ((col == 0 || col == (panes[pane].width - 1)) && (row > 0 && row < (panes[pane].height - 1)))
            {
                panes[pane].virtual_pane[row][col] = '|';
            }

            // panes[pane].virtual_pane[0][0] = '┌';
            // panes[pane].virtual_pane[0][panes[pane].width] = '┐';

            // panes[pane].virtual_pane[panes[pane].height][0] = '└';
            // panes[pane].virtual_pane[panes[pane].height][panes[pane].width] = '┘';
        }
    }
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

inline Curs::~Curs()
{
}

#endif
