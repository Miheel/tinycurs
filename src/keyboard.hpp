#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <unistd.h>
#include "esc_code.hpp"

// class Event;
std::map<std::string, bool> keys;
// std::queue<Event> events;

auto getKey() -> std::string
{
    char keyStr[8];
    int nbytes = 0;
    // char ch;
    // if (!std::cin.get(ch))
    // {
    //     return "";
    // }
    // if (ch == '\033')
    // {
    //     char next1, next2;
    //     if (std::cin.get(next1) && std::cin.get(next2))
    //     {
    //         keyStr.push_back(ch);
    //         keyStr.push_back(next1);
    //         keyStr.push_back(next2);
    //         return keyStr;
    //     }
    // }
    // else
    // {
    //     keyStr.push_back(ch);
    // }
    nbytes = read(STDIN_FILENO, keyStr, 8);
    if (nbytes <= 0)
    {
        return "";
    }
    return std::string(keyStr, nbytes);
}

auto pollKey() -> bool
{
    std::string key = getKey();
    if (!key.empty())
    {
        keys[key] = true;
        return true;
    }
    return false;
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

auto mousePress() -> Mouse
{
    std::string buff;
    char ch;
    Mouse pos;
    while (std::cin.get(ch))
    {
        buff += ch;
        if (ch == 'M')
        {
            break;
        }
    }
    std::sscanf(buff.c_str(), "%d;%d;%d%c", &pos.btn, &pos.col, &pos.row, &pos.event);
    return pos;
}

#endif