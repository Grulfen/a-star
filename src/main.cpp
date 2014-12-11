#include<stdlib.h>
#include<curses.h>
#include"thing_util.hpp"
#include<vector>

using namespace std;

// Class for representing the world
class world {
        public:
                world(int x, int y) : x(x), y(y) {};
                int x;
                int y;
                vector<char> matrix;
};


// Put some sane values into the world
void init_world(world &w)
{
        for(int y=0; y < w.y; y++){
                for(int x=0; x < w.x; x++){
                        if(x > 5 && x < 50 && y == 12){
                                w.matrix.push_back('#');
                        } else {
                                w.matrix.push_back('.');
                        }
                }
        }
}


// Draw world to stdscr (curses)
void draw_world(world &w, int color)
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
                       mvaddch(y + 1, x + 1, c);
                }
        }
        attroff(COLOR_PAIR(color));
}


// Draw thing to stdscr
void draw_thing(thing &t)
{
        attron(COLOR_PAIR(t.color));
        mvaddch(t.y + 1, t.x + 1, t.symbol);
        attroff(COLOR_PAIR(t.color));
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


int tile_passable(world &w, int x, int y)
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


// Moves the goblin towards the player
// TODO use a-star to move better
void move_goblin(thing &g, thing &p, world &w)
{
        int dx = g.x - p.x;
        int dy = g.y - p.y;

        int move_x = dx > 0 ? -1 : 1;
        int move_y = dy > 0 ? -1 : 1;

        if(abs(dx) > abs(dy)){
                if(tile_passable(w, g.x+move_x, g.y)){
                        g.x += move_x;
                } else if(tile_passable(w, g.x, g.y + move_y)){
                        g.y += move_y;
                }
        } else {
                if(tile_passable(w, g.x, g.y + move_y)){
                        g.y += move_y;
                } else if(tile_passable(w, g.x+move_x, g.y)){
                        g.x += move_x;
                }
        }
}

void draw_things(vector<thing> &things)
{
        vector<thing>::iterator it;
        for(it = things.begin(); it != things.end(); ++it){
                draw_thing(*it);
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
        world w = world(col-2, row-2);
        init_world(w);

        // Things init stuff
        vector<thing> things;
        thing player = thing(4, '@', 10, 10);
        thing goblin = thing(3, 'g', 20, 30);
        things.push_back(player);
        things.push_back(goblin);

        int c = 'n';
        while (c != 'q'){
                switch (c) {
                        case KEY_DOWN:
                        case 's':
                                if(tile_passable(w, player.x, player.y+1)){
                                        player.y += 1;
                                }
                                break;
                        case KEY_UP:
                        case 'w':
                                if(tile_passable(w, player.x, player.y-1)){
                                        player.y -= 1;
                                }
                                break;
                        case KEY_LEFT:
                        case 'a':
                                if(tile_passable(w, player.x-1, player.y)){
                                        player.x -= 1;
                                }
                                break;
                        case KEY_RIGHT:
                        case 'd':
                                if(tile_passable(w, player.x+1, player.y)){
                                        player.x += 1;
                                }
                                break;
                        default: 
                                move_goblin(goblin, player, w);
                                break;
                }
                draw_world(w, color); // Draw world to screen
                draw_thing(player); // Draw things to screen
                draw_thing(goblin); // Draw things to screen
                refresh();
                c = getch(); // Check if user input is available - blocking call
        }

        endwin();

        return 0;
}
