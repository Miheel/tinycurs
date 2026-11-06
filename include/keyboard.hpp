#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include <queue>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include "esc_code.hpp"
#include "signalHandler.hpp"
#include "termmode.hpp"

struct Mouse
{
    enum class Button
    {
        Left = 0,
        Middle = 1,
        Right = 2,
        ScrollUp = 64,
        ScrollDown = 65,
        Unknown = 255
    };
    Button btn = Button::Unknown;
    int col = 0;
    int row = 0;
    char event = 'n';
    Button getButton() const
    {
        return btn;
    }
    std::string toString() const
    {
        return "Mouse(btn=" + std::to_string(static_cast<int>(btn)) + ", col=" + std::to_string(col) + ", row=" + std::to_string(row) + ", event=" + event + ")";
    }
};

struct ResizeEvent
{
    int width;
    int height;
};

class Event
{
public:
    enum class Type
    {
        KeyPress,
        MousePress,
        MouseRelease,
        MouseScrollUp,
        MouseScrollDown,
        MouseMove,
        Resize,
        Unknown
    };

    Event() : type(Type::Unknown), key("") {}
    Event(Type t, const std::string &k) : type(t), key(k) {}
    Event(Type t, const Mouse &m) : type(t), key(""), mouse(m) {}
    Event(Type t, const ResizeEvent &r) : type(t), key(""), mouse(), resize(r) {}
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
    ResizeEvent resize;
};

auto getKey() -> std::string;

class Input
{
private:
    std::queue<Event> events;
    std::string buffer;

    void checkResize();

    bool parseMouse(std::string &str);
    bool parseCSI();
    bool parseESC();

    bool parseCharacter();

public:
    Input() : buffer("") {}
    void readInput();
    void parseInput();

    void poll();
    bool hasEvent();
    Event getEvent();
};

#endif
