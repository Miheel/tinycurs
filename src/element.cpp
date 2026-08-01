#include "element.hpp"
#include "pane.hpp"

void BorderElement::render(Pane &target)
{
    for (size_t row = 0; row < target.pos_size.height; row++)
    {
        for (size_t col = 0; col < target.pos_size.width; col++)
        {
            if ((row == 0 || row == (target.pos_size.height - 1)) && (col > 0 && col < (target.pos_size.width - 1)))
            {
                target.virtual_pane[row][col].glyph = borderv;
                target.virtual_pane[row][col].bg = color::Blue;
                target.virtual_pane[row][col].fg = color::White;
            }
            if ((col == 0 || col == (target.pos_size.width - 1)) && (row > 0 && row < (target.pos_size.height - 1)))
            {
                target.virtual_pane[row][col].glyph = borderH;
                target.virtual_pane[row][col].bg = color::Blue;
                target.virtual_pane[row][col].fg = color::White;
            }
        }
    }

    target.virtual_pane[0][0].glyph = topLcornerround;                         //'+';                  //'┌';
    target.virtual_pane[0][target.pos_size.width - 1].glyph = topRcornerround; //'+'; //'┐';
    target.virtual_pane[0][0].bg = color::Blue;
    target.virtual_pane[0][target.pos_size.width - 1].fg = color::White;

    target.virtual_pane[target.pos_size.height - 1][0].glyph = botLcornerround;                         //'+';                  //'└';
    target.virtual_pane[target.pos_size.height - 1][target.pos_size.width - 1].glyph = botRcornerround; //'+'; //'┘';
    target.virtual_pane[target.pos_size.height - 1][0].bg = color::Blue;
    target.virtual_pane[target.pos_size.height - 1][target.pos_size.width - 1].fg = color::White;
}

void TitleElement::render(Pane &target)
{
    target.print(0, 0, title);
}

void TextElement::render(Pane &target)
{
    target.printTextF(row, col, text, fg, bg);
}
