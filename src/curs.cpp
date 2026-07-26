#include "curs.hpp"

Curs::Curs(int width, int height)
{
    init();
    if (_winsize.ws_col < width || _winsize.ws_row < height)
    {
        std::cerr << "Window too small for app.\n";
        throw 1;
    }
}

void Curs::init()
{
    std::cout << Erase::all << CURSHOME;
    initWinsize();
    std::vector<std::vector<Cell>> tempbuff(_winsize.ws_row, std::vector<Cell>(_winsize.ws_col));
    virtual_screen = tempbuff;
    phys_screen = tempbuff;
}

void Curs::initWinsize()
{
    int status = ioctl(STDERR_FILENO, TIOCGWINSZ, &_winsize);
    if (status == -1)
    {
        std::cerr << "error";
    }
}

winsize Curs::getwinsize()
{
    return _winsize;
}

void Curs::addpane(int topX, int topY, int width, int height)
{
    Pane pane(topX, topY, width, height);

    panes.push_back(pane);
}

void Curs::print(int row, int col, std::string str)
{
    for (size_t i = 0; i < str.size() && (col + 1) < _winsize.ws_col; i++)
    {
        virtual_screen[row][col++].glyph = str[i];
    }
}

void Curs::print(int pane, int row, int col, std::string str)
{
    for (size_t i = 0; i < str.size() && (col + 1) < panes[pane].pos_size.width; i++)
    {
        panes[pane].virtual_pane[row][col++].glyph = str[i];
    }
}

void Curs::resize()
{
    init();
}

void Curs::clear()
{
    std::vector<std::vector<Cell>> tempbuff(_winsize.ws_row, std::vector<Cell>(_winsize.ws_col));
    virtual_screen = tempbuff;
}

void Curs::draw(Pane &pane)
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

void Curs::endframe()
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