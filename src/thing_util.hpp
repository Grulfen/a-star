#ifndef THING_UTIL_H
#define THING_UTIL_H
#include<stdlib.h>

// Struct for thing in world
struct thing {
        thing(int color, char symbol, int x, int y) : 
                color(color), symbol(symbol), x(x), y(y) {};
        int color;
        char symbol;
        int x;
        int y;
};

#endif
