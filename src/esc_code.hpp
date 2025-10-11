#ifndef ESC_CODE_HPP
#define ESC_CODE_HPP

#include <string>
#include <string_view>
#include <utility>
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
void cursMoveL(int N)
{
    std::cout << ESC << N << "C"; // ESC[#C	moves cursor right # columns
}
void cursMoveR(int N)
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

struct Erase
{
    // static constexpr std::string_view toEndScr = "\033[J"; // ESC[J	erase in display (same as ESC[0J)
    static constexpr std::string_view toEndScr = "\033[0J";   // ESC[0J	erase from cursor until end of screen
    static constexpr std::string_view fromBegScr = "\033[1J"; // ESC[1J	erase from cursor to beginning of screen
    static constexpr std::string_view all = "\033[2J";        // ESC[2J	erase entire screen
    static constexpr std::string_view savedLines = "\033[3J"; // ESC[3J	erase saved lines
    // static constexpr std::string_view toEndLine = "\033[K"; // ESC[K	erase in line (same as ESC[0K)
    static constexpr std::string_view toEndLine = "\033[0K";   // ESC[0K	erase from cursor to end of line
    static constexpr std::string_view fromBegline = "\033[1K"; // ESC[1K	erase start of line to the cursor
    static constexpr std::string_view line = "\033[2K";        // ESC[2K	erase the entire line
};

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
};

struct Reset
{
    static constexpr std::string_view all = "\033[0m";            // ESC[0m	reset all modes (styles and colors)
    static constexpr std::string_view bold = "\033[22m";          // ESC[22m	reset bold mode.
    static constexpr std::string_view bim = "\033[22m";           // ESC[22m	reset dim/faint mode.
    static constexpr std::string_view italic = "\033[23m";        // ESC[23m	reset italic mode.
    static constexpr std::string_view underline = "\033[24m";     // ESC[24m	reset underline mode.
    static constexpr std::string_view blinking = "\033[25m";      // ESC[25m	reset blinking mode
    static constexpr std::string_view inverse = "\033[27m";       // ESC[27m	reset inverse/reverse mode
    static constexpr std::string_view hidden = "\033[28m";        // ESC[28m	reset hidden/invisible mode
    static constexpr std::string_view strikethrough = "\033[29m"; // ESC[29m	reset strikethrough mode.
};

struct FG
{
    static constexpr std::string_view Black = "\033[30";
    static constexpr std::string_view Red = "\033[31";
    static constexpr std::string_view Green = "\033[32";
    static constexpr std::string_view Yellow = "\033[33";
    static constexpr std::string_view Blue = "\033[34";
    static constexpr std::string_view Magenta = "\033[35";
    static constexpr std::string_view Cyan = "\033[36";
    static constexpr std::string_view White = "\033[37";
    static constexpr std::string_view Default = "\033[39";
};

struct BG
{
    static constexpr std::string_view Black = "\033[40";
    static constexpr std::string_view Red = "\033[41";
    static constexpr std::string_view Green = "\033[42";
    static constexpr std::string_view Yellow = "\033[43";
    static constexpr std::string_view Blue = "\033[44";
    static constexpr std::string_view Magenta = "\033[45";
    static constexpr std::string_view Cyan = "\033[46";
    static constexpr std::string_view White = "\033[47";
    static constexpr std::string_view Default = "\033[49";
};

struct FGbright
{
    static constexpr std::string_view Black = "\033[90";
    static constexpr std::string_view Red = "\033[91";
    static constexpr std::string_view Green = "\033[92";
    static constexpr std::string_view Yellow = "\033[93";
    static constexpr std::string_view Blue = "\033[94";
    static constexpr std::string_view Magenta = "\033[95";
    static constexpr std::string_view Cyan = "\033[96";
    static constexpr std::string_view White = "\033[97";
};

struct BGbright
{
    static constexpr std::string_view Black = "\033[100";
    static constexpr std::string_view Red = "\033[101";
    static constexpr std::string_view Green = "\033[102";
    static constexpr std::string_view Yellow = "\033[103";
    static constexpr std::string_view Blue = "\033[104";
    static constexpr std::string_view Magenta = "\033[105";
    static constexpr std::string_view Cyan = "\033[106";
    static constexpr std::string_view White = "\033[107";
};

void FGcolorID(int id)
{
    std::cout << ESC << "38;5;" << id << "m"; // ESC[38;5;{ID}m	Set foreground color.
}
void BGcolorID(int id)
{
    std::cout << ESC << "48;5;" << id << "m"; // ESC[48;5;{ID}m	Set background color.
}

void FGrgb(int r, int g, int b)
{
    std::cout << ESC << "38;2;" << r << ";" << g << ";" << b << "m"; // ESC[38;2;{r};{g};{b}m	Set foreground color as RGB.
}
void BGrgb(int r, int g, int b)
{
    std::cout << ESC << "48;2;" << r << ";" << g << ";" << b << "m"; // ESC[48;2;{r};{g};{b}m	Set background color as RGB.
}

void cursStyle(curs_mode mode)
{
    std::cout << ESC << mode << " q";
}
constexpr std::string_view curInvis = "\033[?25l";   // ESC[?25l	make cursor invisible
constexpr std::string_view curHid = "\033[?25h";     // ESC[?25h	make cursor visible
constexpr std::string_view restoreScr = "\033[?47l"; // ESC[?47l	restore screen
constexpr std::string_view saveScr = "\033[?47h";    // ESC[?47h	save screen
// ESC[?1049h	enables the alternative buffer
// ESC[?1049l	disables the alternative buffer

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

#endif
