#ifndef THING_UTIL_H
#define THING_UTIL_H
#include<stdlib.h>
#include<utility>

// Struct for thing in world
struct thing {
        thing(int color, char symbol, int x, int y) : 
                color(color), symbol(symbol), pos({x, y}) {};
        int color;
        char symbol;
        std::pair<int, int> pos;
};

#endif
