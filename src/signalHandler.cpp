#include "signalHandler.hpp"

void sigResize(int sig)
{
    resized = true;
}