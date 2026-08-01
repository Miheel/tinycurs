#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <unistd.h>
#include "esc_code.hpp"

// class Event;
inline std::map<std::string, bool> keys;
// std::queue<Event> events;

auto getKey() -> std::string;

auto pollKey() -> bool;

auto isKeyPressed(const std::string &key) -> bool;

auto mousePress() -> Mouse;

#endif
