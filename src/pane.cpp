#include "pane.hpp"

Pane::Pane(int topX, int topY, int width, int height)
    : virtual_pane(height, std::vector<Cell>(width))
{
    pos_size.topX = topX;
    pos_size.topY = topY;
    pos_size.width = width;
    pos_size.height = height;
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

std::vector<std::vector<Cell>> &Pane::getVirtualPane()
{
    return this->virtual_pane;
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
        if (str[i] == ' ')
        {
            this->virtual_pane[row][col].bg = bg;
            this->virtual_pane[row][col++].fg = fg;
        }
        else
        {
            this->virtual_pane[row][col].glyph = str[i];
            this->virtual_pane[row][col].bg = bg;
            this->virtual_pane[row][col++].fg = fg;
        }
    }
}

void Pane::clear()
{
    std::vector<std::vector<Cell>> tempbuff(getPos_Size().height, std::vector<Cell>(getPos_Size().width));
    virtual_pane = tempbuff;
}

void Pane::render()
{
    clear();
    for (auto element : elements)
    {
        element->render(*this);
    }

    for (auto childPane : child_panes)
    {
        childPane->render();
    }
}
