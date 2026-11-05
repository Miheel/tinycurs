#include "keyboard.hpp"
#include <sys/ioctl.h>

auto getKey() -> std::string
{
    int nbytes = 0;
    ioctl(STDIN_FILENO, FIONREAD, &nbytes);
    std::string buff(nbytes, '\0');
    size_t nbytesRead = read(STDIN_FILENO, buff.data(), nbytes);

    if (nbytesRead <= 0)
    {
        return std::string();
    }

    return buff;
}

bool Input::parseMouse(std::string &str)
{
    Mouse mouse;
    std::string buff;

    std::sscanf(str.c_str(), "\033[<%d;%d;%d%c", &mouse.btn, &mouse.col, &mouse.row, &mouse.event);

    if (mouse.event == 'M' && mouse.btn == Mouse::Button::ScrollUp)
    {
        events.emplace(Event::Type::MouseScrollUp, mouse);
        return true;
    }
    else if (mouse.event == 'M' && mouse.btn == Mouse::Button::ScrollDown)
    {
        events.emplace(Event::Type::MouseScrollDown, mouse);
        return true;
    }
    else if (mouse.event == 'M')
    {
        events.emplace(Event::Type::MousePress, mouse);
        return true;
    }
    else if (mouse.event == 'm')
    {
        events.emplace(Event::Type::MouseRelease, mouse);
        return true;
    }
    return false;
}

bool Input::parseCSI()
{
    size_t end = 2;
    for (; end < buffer.size(); end++)
    {
        if (buffer[end] >= '@' && buffer[end] <= '~')
        {
            break;
        }
    }
    if (end >= buffer.size())
    {
        return false;
    }
    std::string csi = buffer.substr(0, end + 1);

    switch (csi.back())
    {
    case 'A':
        events.emplace(Event::Type::KeyPress, UP_KEY);
        break;
    case 'B':
        events.emplace(Event::Type::KeyPress, DOWN_KEY);
        break;
    case 'C':
        events.emplace(Event::Type::KeyPress, LEFT_KEY);
        break;
    case 'D':
        events.emplace(Event::Type::KeyPress, RIGHT_KEY);
        break;
    case 'M':
    case 'm':
        parseMouse(csi);
        break;

    default:
        return false;
        break;
    }
    buffer.erase(0, end + 1);
    return true;
}

bool Input::parseESC()
{
    if (buffer.size() < 2)
    {
        return false;
    }
    switch (buffer[1])
    {
    case '[':
        return parseCSI();
    default:
        return false;
    }
}

void Input::readInput()
{
    buffer = getKey();
}

void Input::parseInput()
{
    if (buffer.empty())
    {
        return;
    }
    switch (buffer[0])
    {
    case '\033':
        parseESC();
        break;
    default:
        parseCharacter();
        break;
    }
}

void Input::poll()
{
    readInput();
    parseInput();
}

bool Input::hasEvent()
{
    return !events.empty();
}

Event Input::getEvent()
{
    if (events.empty())
    {
        return Event(Event::Type::Unknown, std::string());
    }
    Event e = events.front();
    events.pop();
    return e;
}
