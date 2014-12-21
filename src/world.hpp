#ifndef WORLD_HPP
#define WORLD_HPP

#include<vector>
// Class for representing the world
class World {
    public:
        World(int x, int y);
        int x;
        int y;
        std::vector<char> matrix;
        bool passable(std::pair<int, int> pos);
};
#endif
