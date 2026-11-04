#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <unistd.h>
#include "esc_code.hpp"

inline std::map<std::string, bool> keys;
// std::queue<Event> events;
//  event class for keyboard and mouse events
struct Mouse;
class Event
{
public:
    enum class Type
    {
        KeyPress,
        MousePress,
        MouseRelease,
        MouseMove,
        Resize,
        Unknown
    };

    Event() : type(Type::Unknown), key("") {}
    Event(Type t, const std::string &k) : type(t), key(k) {}
    Event(Type t, const Mouse &m) : type(t), mouse(m) {}
    Event(const Event &e) : type(e.type), key(e.key), mouse(e.mouse) {}
    Event &operator=(const Event &e)
    {
        if (this != &e)
        {
            type = e.type;
            key = e.key;
            mouse = e.mouse;
        }
        return *this;
    }
    Type getType() const { return type; }
    std::string getKey() const { return key; }
    Mouse getMouse() const { return mouse; }

private:
    Type type;
    std::string key;
    Mouse mouse;
};

auto getKey() -> std::string;

auto pollKey() -> Event;

auto isKeyPressed(const std::string &key) -> bool;

auto mousePress(const std::string &key) -> Mouse;

#endif
