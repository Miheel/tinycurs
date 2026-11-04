#include "keyboard.hpp"
#include <sys/ioctl.h>

auto getKey() -> std::string
{
    std::vector<char> keyStr;
    int nbytes = 0;
    char ch;
    ioctl(STDIN_FILENO, FIONREAD, &nbytes);

    for (int i = 0; i < nbytes; i++)
    {
        ch = getchar();
        keyStr.push_back(ch);
    }

    if (keyStr.empty())
    {
        return std::string();
    }

    return std::string(keyStr.begin(), keyStr.end());
}

auto pollKey() -> Event
{
    std::string key = getKey();
    if (key.empty())
    {
        return Event(Event::Type::Unknown, std::string());
    }
    if (key[0] != '\033' && key.size() == 1)
    {
        keys[key] = true;
        return Event(Event::Type::KeyPress, key);
    }
    if (key[0] == '\033' && key.size() > 1)
    {
        if (key[1] == '[' && key.size() > 2)
        {
            if (key[2] == '<')
            {
                auto mouse = mousePress(key);
                if (mouse.event == 'M')
                {
                    return Event(Event::Type::MousePress, mouse);
                }
                else if (mouse.event == 'm')
                {
                    return Event(Event::Type::MouseRelease, mouse);
                }
            }
            else if (key[2] == 'R')
            {
                return Event(Event::Type::Resize, key);
            }
            else
            {
                return Event(Event::Type::KeyPress, key);
            }
        }
    }
    return Event(Event::Type::Unknown, std::string());
}

auto isKeyPressed(const std::string &key) -> bool
{
    auto pressed = keys[key];
    if (pressed)
    {
        keys[key] = false;
    }
    return pressed;
}

auto mousePress(const std::string &key) -> Mouse
{
    Mouse pos;
    std::string buff;
    for (auto &c : key)
    {
        buff += c;
        if (c == 'M' || c == 'm')
        {
            break;
        }
    }
    std::sscanf(key.c_str(), "\033[<%d;%d;%d%c", &pos.btn, &pos.col, &pos.row, &pos.event);
    return pos;
}
