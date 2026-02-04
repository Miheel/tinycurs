#ifndef ESC_CODE_HPP
#define ESC_CODE_HPP

#include <iostream>
#include <cstdio>
#include <string>
#include <string_view>
#include <vector>

enum curs_mode
{
    block,
    fulblock,
    blinkblock,
    underline,
    blinkunderline,
    bar,
    blinkbar
};

struct CurPos
{
    int line;
    int col;
};

struct Mouse
{
    int btn;
    int col;
    int row;
    char event;
};

struct RGB
{
    unsigned int r;
    unsigned int g;
    unsigned int b;
};

constexpr std::string_view ESC = "\033[";
constexpr std::string_view OSC = "\033]";
constexpr std::string_view ST = "\033\\";

constexpr std::string_view CURSHOME = "\033[H"; // ESC[H	moves cursor to home position (0, 0)

void cursHome()
{
    std::cout << CURSHOME;
}

void cursMove(int row, int col)
{
    std::cout << ESC << row << ";" << col << "H"; // ESC[{line};{column}H
}
void cursMoveU(int N)
{
    std::cout << ESC << N << "A"; // ESC[#A	moves cursor up # lines
}
void cursMoveD(int N)
{
    std::cout << ESC << N << "B"; // ESC[#B	moves cursor down # lines
}
void cursMoveR(int N)
{
    std::cout << ESC << N << "C"; // ESC[#C	moves cursor right # columns
}
void cursMoveL(int N)
{
    std::cout << ESC << N << "D"; // ESC[#D	moves cursor left # columns
}

void cursBegNextLine(int N)
{
    std::cout << ESC << N << "E"; // ESC[#E	moves cursor to beginning of next line, # lines down
}
void cursBegPrevLine(int N)
{
    std::cout << ESC << N << "F"; // ESC[#F	moves cursor to beginning of previous line, # lines up
}
void cursColN(int N)
{
    std::cout << ESC << N << "G"; // ESC[#G	moves cursor to column #
}
auto cursPos() -> CurPos
{
    std::string buff;
    char ch;
    CurPos pos;
    std::cout << ESC << "6n"; // ESC[6n	request cursor position (reports as ESC[#;#R)'
    while (std::cin.get(ch))
    {
        buff += ch;
        if (ch == 'R')
        {
            break;
        }
    }
    std::sscanf(buff.c_str(), "\033[%d;%dR", &pos.line, &pos.col);
    return pos;
}

constexpr std::string_view moveUpscroll = "\033M"; // ESC M	moves cursor one line up, scrolling if needed
// constexpr std::string_view curSavePos = "\0337";     // ESC 7	save cursor position (DEC)
// constexpr std::string_view corRestorePos = "\0338";  // ESC 8	restores the cursor to the last saved position (DEC)
constexpr std::string_view curSavePos = "\033[s";    // ESC[s	save cursor position (SCO)
constexpr std::string_view curRestorePos = "\033[u"; // ESC[u	restores the cursor to the last saved position (SCO)

void cursStyle(curs_mode mode)
{
    std::cout << ESC << mode << " q";
}
constexpr std::string_view curInvis = "\033[?25l";   // ESC[?25l	make cursor invisible
constexpr std::string_view curVis = "\033[?25h";     // ESC[?25h	make cursor visible
constexpr std::string_view restoreScr = "\033[?47l"; // ESC[?47l	restore screen
constexpr std::string_view saveScr = "\033[?47h";    // ESC[?47h	save screen
// ESC[?1049h	enables the alternative buffer
// ESC[?1049l	disables the alternative buffer

struct Erase
{
    // static constexpr std::string_view toEndScr = "\033[J"; // ESC[J	erase in display (same as ESC[0J)
    static constexpr std::string_view toEndScr = "\033[0J";   // ESC[0J	erase from cursor until end of screen
    static constexpr std::string_view toBegScr = "\033[1J";   // ESC[1J	erase from cursor to beginning of screen
    static constexpr std::string_view all = "\033[2J";        // ESC[2J	erase entire screen
    static constexpr std::string_view savedLines = "\033[3J"; // ESC[3J	erase saved lines
    // static constexpr std::string_view toEndLine = "\033[K"; // ESC[K	erase in line (same as ESC[0K)
    static constexpr std::string_view toEndLine = "\033[0K";   // ESC[0K	erase from cursor to end of line
    static constexpr std::string_view fromBegline = "\033[1K"; // ESC[1K	erase start of line to the cursor
    static constexpr std::string_view line = "\033[2K";        // ESC[2K	erase the entire line

    enum class Mode
    {
        toEndScr,
        toBegScr,
        all,
        savedLines,
        toEndLine,
        fromBegline,
        line
    };
};

void erase(Erase::Mode mode)
{
    std::string eraseCode;
    switch (mode)
    {
    case Erase::Mode::toEndScr:
        eraseCode = Erase::toEndScr;
        break;
    case Erase::Mode::toBegScr:
        eraseCode = Erase::toBegScr;
        break;
    case Erase::Mode::all:
        eraseCode = Erase::all;
        break;
    case Erase::Mode::savedLines:
        eraseCode = Erase::savedLines;
        break;
    case Erase::Mode::toEndLine:
        eraseCode = Erase::toEndLine;
        break;
    case Erase::Mode::fromBegline:
        eraseCode = Erase::fromBegline;
        break;
    case Erase::Mode::line:
        eraseCode = Erase::line;
        break;
    default:
        break;
    }

    std::cout << eraseCode;
}

void clearscr()
{
    erase(Erase::Mode::all);
    cursHome();
}

struct Style
{
    // ESC[1;34;{...}m		Set graphics modes for cell, separated by semicolon (;).

    static constexpr std::string_view bold = "\033[1m";          // ESC[1m	set bold mode.
    static constexpr std::string_view dim = "\033[2m";           // ESC[2m	set dim/faint mode.
    static constexpr std::string_view italic = "\033[3m";        // ESC[3m	set italic mode.
    static constexpr std::string_view underline = "\033[4m";     // ESC[4m	set underline mode.
    static constexpr std::string_view blinking = "\033[5m";      // ESC[5m	set blinking mode
    static constexpr std::string_view inverse = "\033[7m";       // ESC[7m	set inverse/reverse mode
    static constexpr std::string_view hidden = "\033[8m";        // ESC[8m	set hidden/invisible mode
    static constexpr std::string_view strikethrough = "\033[9m"; // ESC[9m	set strikethrough mode.

    enum class Mode : uint16_t
    {
        bold = 1 << 0,
        dim = 1 << 1,
        italic = 1 << 2,
        underline = 1 << 3,
        blinking = 1 << 4,
        inverse = 1 << 5,
        hidden = 1 << 6,
        strikethrough = 1 << 7
    };
};

bool operator&(Style::Mode a, Style::Mode b)
{
    using utype = std::underlying_type<Style::Mode>::type;
    return (static_cast<utype>(a) & static_cast<utype>(b));
}

Style::Mode operator|(Style::Mode a, Style::Mode b)
{
    using utype = std::underlying_type<Style::Mode>::type;
    return static_cast<Style::Mode>(static_cast<utype>(a) | static_cast<utype>(b));
}

void setStyle(Style::Mode mode)
{
    std::string styleCode;

    if (mode & Style::Mode::bold)
    {
        styleCode += Style::bold;
    }
    if (mode & Style::Mode::dim)
    {
        styleCode += Style::dim;
    }
    if (mode & Style::Mode::italic)
    {
        styleCode += Style::italic;
    }
    if (mode & Style::Mode::underline)
    {
        styleCode += Style::underline;
    }
    if (mode & Style::Mode::blinking)
    {
        styleCode += Style::blinking;
    }
    if (mode & Style::Mode::inverse)
    {
        styleCode += Style::inverse;
    }
    if (mode & Style::Mode::hidden)
    {
        styleCode += Style::hidden;
    }
    if (mode & Style::Mode::strikethrough)
    {
        styleCode += Style::strikethrough;
    }

    std::cout << styleCode;
}

struct Reset
{
    static constexpr std::string_view all = "\033[0m";            // ESC[0m	reset all modes (styles and colors)
    static constexpr std::string_view bold = "\033[22m";          // ESC[22m	reset bold mode.
    static constexpr std::string_view dim = "\033[22m";           // ESC[22m	reset dim/faint mode.
    static constexpr std::string_view italic = "\033[23m";        // ESC[23m	reset italic mode.
    static constexpr std::string_view underline = "\033[24m";     // ESC[24m	reset underline mode.
    static constexpr std::string_view blinking = "\033[25m";      // ESC[25m	reset blinking mode
    static constexpr std::string_view inverse = "\033[27m";       // ESC[27m	reset inverse/reverse mode
    static constexpr std::string_view hidden = "\033[28m";        // ESC[28m	reset hidden/invisible mode
    static constexpr std::string_view strikethrough = "\033[29m"; // ESC[29m	reset strikethrough mode.
};

void resetStyle(Style::Mode mode)
{
    std::string resetCode;

    if (mode & Style::Mode::bold)
    {
        resetCode += Reset::bold;
    }
    if (mode & Style::Mode::dim)
    {
        resetCode += Reset::dim;
    }
    if (mode & Style::Mode::italic)
    {
        resetCode += Reset::italic;
    }
    if (mode & Style::Mode::underline)
    {
        resetCode += Reset::underline;
    }
    if (mode & Style::Mode::blinking)
    {
        resetCode += Reset::blinking;
    }
    if (mode & Style::Mode::inverse)
    {
        resetCode += Reset::inverse;
    }
    if (mode & Style::Mode::hidden)
    {
        resetCode += Reset::hidden;
    }
    if (mode & Style::Mode::strikethrough)
    {
        resetCode += Reset::strikethrough;
    }

    std::cout << resetCode;
}

void resetAllStyle()
{
    std::cout << Reset::all;
}

struct FG
{
    static constexpr std::string_view Black = "\033[30m";
    static constexpr std::string_view Red = "\033[31m";
    static constexpr std::string_view Green = "\033[32m";
    static constexpr std::string_view Yellow = "\033[33m";
    static constexpr std::string_view Blue = "\033[34m";
    static constexpr std::string_view Magenta = "\033[35m";
    static constexpr std::string_view Cyan = "\033[36m";
    static constexpr std::string_view White = "\033[37m";
    static constexpr std::string_view Default = "\033[39m";
};

struct BG
{
    static constexpr std::string_view Black = "\033[40m";
    static constexpr std::string_view Red = "\033[41m";
    static constexpr std::string_view Green = "\033[42m";
    static constexpr std::string_view Yellow = "\033[43m";
    static constexpr std::string_view Blue = "\033[44m";
    static constexpr std::string_view Magenta = "\033[45m";
    static constexpr std::string_view Cyan = "\033[46m";
    static constexpr std::string_view White = "\033[47m";
    static constexpr std::string_view Default = "\033[49m";
};

struct FGbright
{
    static constexpr std::string_view Black = "\033[90m";
    static constexpr std::string_view Red = "\033[91m";
    static constexpr std::string_view Green = "\033[92m";
    static constexpr std::string_view Yellow = "\033[93m";
    static constexpr std::string_view Blue = "\033[94m";
    static constexpr std::string_view Magenta = "\033[95m";
    static constexpr std::string_view Cyan = "\033[96m";
    static constexpr std::string_view White = "\033[97m";
};

struct BGbright
{
    static constexpr std::string_view Black = "\033[100m";
    static constexpr std::string_view Red = "\033[101m";
    static constexpr std::string_view Green = "\033[102m";
    static constexpr std::string_view Yellow = "\033[103m";
    static constexpr std::string_view Blue = "\033[104m";
    static constexpr std::string_view Magenta = "\033[105m";
    static constexpr std::string_view Cyan = "\033[106m";
    static constexpr std::string_view White = "\033[107m";
};

void emitFGcolorID(int id)
{
    std::cout << ESC << "38;5;" << id << "m"; // ESC[38;5;{ID}m	Set foreground color.
}
void emitBGcolorID(int id)
{
    std::cout << ESC << "48;5;" << id << "m"; // ESC[48;5;{ID}m	Set background color.
}
void emitColorID(int fg, int bg)
{
    emitFGcolorID(fg);
    emitBGcolorID(bg);
}

std::string FGcolorIDS(int id)
{
    return "\033[38;5;" + std::to_string(id) + "m"; // ESC[38;5;{ID}m	Set foreground color.
}
std::string BGcolorIDS(int id)
{
    return "\033[48;5;" + std::to_string(id) + "m"; // ESC[48;5;{ID}m	Set background color.
}
std::string colorID(int fg, int bg)
{
    return FGcolorIDS(fg) + BGcolorIDS(bg);
}

void FGrgb(int r, int g, int b)
{
    std::cout << ESC << "38;2;" << r << ";" << g << ";" << b << "m"; // ESC[38;2;{r};{g};{b}m	Set foreground color as RGB.
}
void BGrgb(int r, int g, int b)
{
    std::cout << ESC << "48;2;" << r << ";" << g << ";" << b << "m"; // ESC[48;2;{r};{g};{b}m	Set background color as RGB.
}

/************NOT WORKING****************/
void scrollRegion(int top, int bot)
{
    std::cout << ESC << top << ";" << bot << "r";
}
void scrollRegionTop(int top)
{
    std::cout << ESC << top << "r";
}
void resetScrollRegion()
{
    std::cout << ESC << "r";
}
/***************************************/

static constexpr std::string_view BpasteOn = "\033[?2004h";
static constexpr std::string_view BpasteOff = "\033[?2004l";

auto colorPalet16() -> std::vector<RGB>
{
    const int color_palet = 15;
    RGB rgb;
    std::vector<RGB> rgbvec;
    std::string buff;
    int index;
    char ch;
    for (size_t i = 0; i <= color_palet; i++)
    {
        std::cout << OSC << "4;" << i << ";?" << ST;

        while (std::cin.get(ch))
        {
            buff += ch;
            if (ch == '\033' && std::cin.peek() == '\\')
            {
                std::cin.get(ch);
                buff += ch;
                break;
            }
        }
        std::sscanf(buff.c_str(), "\033]4;%d;rgb:%x/%x/%x\033\\", &index, &rgb.r, &rgb.g, &rgb.b);
        rgbvec.push_back(rgb);

        buff.clear();
    }
    return rgbvec;
}

std::string UP_KEY = "\033[A";
std::string DOWN_KEY = "\033[B";
std::string LEFT_KEY = "\033[C";
std::string RIGHT_KEY = "\033[D";
std::string ENTER_KEY = "\n";
std::string RESIZE = "\033[R";

#endif
