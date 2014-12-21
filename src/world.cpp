#include"world.hpp"

World::World(int x, int y) : x(x), y(y)
{
    for(int y=0; y < this->y; y++){
        for(int x=0; x < this->x; x++){
            if(x > 5 && x < 50 && y == 12){
                matrix.push_back('#');
            } else {
                matrix.push_back('`');
            }
        }
    }
}

bool World::passable(std::pair<int, int> pos)
{
    if(pos.first >= this->x || pos.first < 0 || pos.second >= this->y || pos.second < 0){
        return false;
    }
    switch(matrix[pos.second*this->x + pos.first]){
        case '#':
            return false;
            break;
        default:
            return true;
            break;
    }
}
