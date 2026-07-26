#include "pane.hpp"

Pane::Pane(int topX, int topY, int width, int height)
    : virtual_pane(height, std::vector<Cell>(width))
{
    pos_size.topX = topX;
    pos_size.topY = topY;
    pos_size.width = width;
    pos_size.height = height;
}

void Pane::addBorder()
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
    this->virtual_pane[0][0].bg = color::Blue;
    this->virtual_pane[0][pos_size.width - 1].fg = color::White;

    virtual_pane[pos_size.height - 1][0].glyph = botLcornerround;                  //'+';                  //'└';
    virtual_pane[pos_size.height - 1][pos_size.width - 1].glyph = botRcornerround; //'+'; //'┘';
    this->virtual_pane[pos_size.height - 1][0].bg = color::Blue;
    this->virtual_pane[pos_size.height - 1][pos_size.width - 1].fg = color::White;
}

void Pane::setTitle(std::string title)
{
    this->title = title;
}

void Pane::setpos(int x, int y)
{
    pos_size.topX = x;
    pos_size.topY = y;
}

Pos_size &Pane::getPos_Size()
{
    return this->pos_size;
}

void Pane::print(int row, int col, std::string str)
{
    for (size_t i = 0; i < str.size() && (col + 1) < this->pos_size.width; i++)
    {
        this->virtual_pane[row][col++].glyph = str[i];
    }
}

void Pane::printTextF(int row, int col, std::string str, color bg, color fg)
{
    for (size_t i = 0; i < str.size() && (col + 1) < this->pos_size.width; i++)
    {
        this->virtual_pane[row][col].glyph = str[i];
        this->virtual_pane[row][col].bg = bg;
        this->virtual_pane[row][col++].fg = fg;
    }
}
