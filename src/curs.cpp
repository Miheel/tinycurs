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
    signal(SIGWINCH, sigResize);

    std::cout << Erase::all << CURSHOME;
    initWinsize();
    this->clear();
    phys_screen = virtual_screen;
    isRuning = true;
}

void Curs::deinit()
{
    resetAllStyle();
    std::cout << Erase::all << CURSHOME;
    std::cout << curVis;
    exit();
}

void Curs::initWinsize()
{
    _winsize = termMode.getWindowSize();
}

void Curs::initTerm()
{
    termMode.echoOff();
    termMode.noBlock();
}

winsize Curs::getWinsize()
{
    return _winsize;
}

void Curs::print(int row, int col, std::string str)
{
    for (size_t i = 0; i < str.size() && (col + 1) < _winsize.ws_col; i++)
    {
        virtual_screen[row][col++].glyph = str[i];
    }
}

void Curs::resize()
{
    init();
}

bool Curs::is_runing()
{
    return isRuning;
}

void Curs::exit()
{
    isRuning = false;
}

void Curs::clear()
{
    std::vector<std::vector<Cell>> tempbuff(_winsize.ws_row, std::vector<Cell>(_winsize.ws_col));
    virtual_screen = tempbuff;
}

void Curs::draw(const Pane &pane)
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
                if (pane.virtual_pane[pr][pc].glyph[0] == ' ')
                {
                    virtual_screen[row][col].fg = pane.virtual_pane[pr][pc].fg;
                    virtual_screen[row][col].bg = pane.virtual_pane[pr][pc].bg;
                }
                else
                {
                    virtual_screen[row][col] = pane.virtual_pane[pr][pc];
                }
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

Curs::~Curs()
{
    std::cerr << "Curs destructor called\n";
    deinit();
}
