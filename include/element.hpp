#ifndef ELEMENT_HPP
#define ELEMENT_HPP

#include <string>
#include "util.hpp"
class Pane;

class Element
{
protected:
    int row, col;

public:
    void setpos(int x, int y)
    {
        row = y;
        col = x;
    }
    virtual void render(Pane &target) = 0;
    virtual ~Element() = default;
};

class BorderElement : public Element
{
private:
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

public:
    void render(Pane &target) override;
};

class TitleElement : public Element
{
public:
    TitleElement(const std::string &title) : title(title) {}
    void render(Pane &target) override;
    std::string title;
};

class TextElement : public Element
{
public:
    TextElement(int row, int col, const std::string &text, color fg, color bg)
        : text(text), fg(fg), bg(bg)
    {
        this->row = row;
        this->col = col;
    }
    void render(Pane &target) override;
    std::string text;
    color fg;
    color bg;
};

#endif // ELEMENT_HPP
