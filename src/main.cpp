#include<stdlib.h>
#include<curses.h>
#include<vector>
#include<set>
#include<map>
#include<list>
#include<iostream>

#include"thing_util.hpp"
#include"world.hpp"

using namespace std;


// Return the manhattan distance between start and goal
int cost_estimate(pair<int, int> start, pair<int, int> goal)
{
    return abs(start.first - goal.first) + abs(start.second - goal.second);
}

// Draw world to stdscr (curses)
void draw_world(World &w, int color)
{
    char c;

    // Print border
    attron(COLOR_PAIR(1));
    mvaddch(0, 0, '\\');
    mvaddch(w.y + 1, w.x + 1, '\\');
    mvaddch(0, w.x + 1, '/');
    mvaddch(w.y + 1, 0, '/');
    for(int i=1; i<w.x + 1; i++){
        mvaddch(0, i, '=');
        mvaddch(w.y + 1, i, '=');
    }
    for(int j=1; j<w.y + 1; j++){
        mvaddch(j, 0, '|');
        mvaddch(j, w.x + 1, '|');
    }
    attroff(COLOR_PAIR(1));

    // Print world
    attron(COLOR_PAIR(color));
    for(int y = 0; y < w.y; y++){
        for(int x = 0; x < w.x; x++){
            c = w.matrix[y*w.x + x];
            if(c == '`')
            {
                attroff(COLOR_PAIR(1));
            } else {
                attron(COLOR_PAIR(color));
            }
            mvaddch(y + 1, x + 1, c);
        }
    }
    attroff(COLOR_PAIR(color));
}


// Draw thing to stdscr
void draw_thing(thing *t)
{
    if(t == nullptr){
        return;
    }

    attron(COLOR_PAIR(t->color));
    mvaddch(t->pos.second + 1, t->pos.first + 1, t->symbol);
    attroff(COLOR_PAIR(t->color));
}


// Initialize curses window
void init_curses()
{
    initscr();
    keypad(stdscr, true);
    noecho();
    cbreak();
    // Hide cursos
    curs_set(0);
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
}


int tile_passable(World &w, int x, int y)
{
    if(x >= w.x || x < 0 || y >= w.y || y < 0){
        return 0;
    }
    switch(w.matrix[y*w.x + x]){
        case '.':
            return 1;
            break;
        case '#':
            return 0;
            break;
        default:
            return 1;
            break;
    }
}

pair<int, int> get_best_node(set<pair<int, int> > nodes, pair<int, int> target)
{
    pair<int, int> best_node = {0,0}; 
    int best_score = 999;

    pair<int, int> node;
    int score;

    for(auto const &it : nodes){
        node = it;
        score = cost_estimate(it, target);
        if(score < best_score){
            best_score = score;
            best_node = node;
        }
    }
    return best_node;
}

list<pair<int, int>> reconstruct_path(map<pair<int, int>, pair<int, int> > &came_from, pair<int, int> pos)
{
    pair<int, int> current = pos;
    list<pair<int, int> > total_path = {current};
    while (came_from.find(current) != came_from.end()){
        current = came_from[current];
        total_path.push_front(current);
    }
    total_path.pop_front();
    return total_path;
}

vector<pair<int, int>> get_neighbours(pair<int, int> &node, World& w)
{
    vector<pair<int, int>> neighbours = {};
    vector<pair<int, int>> directions = {{0,1}, {0,-1}, {1,0},{-1,0}};
    pair<int, int> current;
    for(auto const &it : directions){
        current = {node.first + it.first, node.second + it.second};
        if(w.passable(current)){
            neighbours.push_back(current);
        }
    }
    return neighbours;
}

// Moves the thing t towards the thing target
void move_thing(thing &start, thing &target, World &w)
{
    int tmp_g_score;
    set<pair<int, int> > visited {};
    set<pair<int, int> > open_set = {start.pos};
    map<pair<int, int>, pair<int, int> > came_from = {};

    map<pair<int, int>, int> g_score, f_score;

    // Cost from start along best known path
    g_score[start.pos] = 0;

    // Estimated cost from start to goal
    f_score[start.pos] = g_score[start.pos] + cost_estimate(start.pos, target.pos);

    pair<int, int> current;

    while (!open_set.empty()) {
        // Get best candidate
        current = get_best_node(open_set, target.pos);
        
        if(current == target.pos){
            // TODO kan vara idé att fixa så man spar pathen istället för att
            // räkna om den varje gång
            start.pos = reconstruct_path(came_from, current).front();
            /* cerr << "(" << start.pos.first << ", " << start.pos.second << ")" << endl; */
            return;
        } else {
            // Remove from open_set
            open_set.erase(current);

            // Add to visited
            visited.insert(current);

            for(auto neighbour: get_neighbours(current, w)){
                // If neighbour not visited
                if(visited.find(neighbour) != visited.end()){
                    continue;
                }
                tmp_g_score = g_score[current] + 1;
                if((open_set.find(neighbour) == open_set.end()) || tmp_g_score < g_score[neighbour]){
                    came_from[neighbour] = current;
                    g_score[neighbour] = tmp_g_score;
                    f_score[neighbour] = tmp_g_score + cost_estimate(neighbour, target.pos);
                    if(open_set.find(neighbour) == open_set.end()){
                        open_set.insert(neighbour);
                    }
                }
            }
        }
    }
}


// Draw all things in list
void draw_things(vector<thing*> &things)
{
    for(auto const &it : things){
        draw_thing(it);
    }
}


int main()
{
    int color = 2;
    int row, col;

    // Curses initstuff
    init_curses();
    getmaxyx(stdscr, row, col);

    // World init stuff
    World w = World(col-2, row-2);

    // Things init stuff
    vector<thing*> things;
    thing player = thing(4, '@', 10, 10);
    thing goblin = thing(3, 'g', 20, 30);
    things.push_back(&player);
    things.push_back(&goblin);

    int c = 'n';
    while (c != 'q'){
        switch (c) {
            case KEY_DOWN:
            case 's':
                player.pos.second += 1;
                if(!w.passable(player.pos)){
                    player.pos.second -= 1;
                }
                break;
            case KEY_UP:
            case 'w':
                    player.pos.second -= 1;
                if(!w.passable(player.pos)){
                    player.pos.second += 1;
                }
                break;
            case KEY_LEFT:
            case 'a':
                player.pos.first -= 1;
                if(!w.passable(player.pos)){
                    player.pos.first += 1;
                }
                break;
            case KEY_RIGHT:
            case 'd':
                player.pos.first += 1;
                if(!w.passable(player.pos)){
                    player.pos.first -= 1;
                }
                break;
            case ' ':
                move_thing(goblin, player, w);
                break;
        }
        draw_world(w, color); // Draw world to screen
        draw_things(things);
        refresh();
        c = getch(); // Check if user input is available - blocking call
    }

    endwin();

    return 0;
}
