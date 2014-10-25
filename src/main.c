#include<stdlib.h>
#include<curses.h>
#include"thing_util.h"

// Struct for representing the world
typedef struct {
        int x;
        int y;
        char *matrix;
} world;

// Create a new world initialized to 0
world* create_world(int x, int y)
{
        world* w = malloc(sizeof(world));
        w->x = x;
        w->y = y;
        w->matrix = calloc(x*y, sizeof(char)); // Use calloc for initialized values
        return w;
}

// Free the world
void destroy_world(world *w)
{
        free(w->matrix);
        free(w);
}

// Put some sane values into the world
void init_world(world *w)
{
        for(int y=0; y < w->y; y++){
                for(int x=0; x < w->x; x++){
                        w->matrix[y*w->x + x] = '.';
                }
        }
}

// Print world to stdout
void print_world(world *w)
{
        for(int y = 0; y < w->y; y++){
                for(int x = 0; x < w->x; x++){
                        if(w->matrix[y*w->x + x]){
                                putchar('#');
                        } else {
                                putchar('.');
                        }
                }
                putchar('\n');
        }
        putchar('\n');
}

// Draw world to stdscr (curses)
void draw_world(world *w, int color)
{
        char c;

        // Print border
        attron(COLOR_PAIR(1));
        mvaddch(0, 0, '\\');
        mvaddch(w->y + 1, w->x + 1, '\\');
        mvaddch(0, w->x + 1, '/');
        mvaddch(w->y + 1, 0, '/');
        for(int i=1; i<w->x + 1; i++){
                mvaddch(0, i, '=');
                mvaddch(w->y + 1, i, '=');
        }
        for(int j=1; j<w->y + 1; j++){
                mvaddch(j, 0, '|');
                mvaddch(j, w->x + 1, '|');
        }
        attroff(COLOR_PAIR(1));

        // Print world
        attron(COLOR_PAIR(color));
        for(int y = 0; y < w->y; y++){
                for(int x = 0; x < w->x; x++){
                       c = w->matrix[y*w->x + x];
                       mvaddch(y + 1, x + 1, c);
                }
        }
        attroff(COLOR_PAIR(color));
}

// Draw thing on stdscr
void draw_thing(thing *t)
{
        if(t == NULL){
                return;
        }

        attron(COLOR_PAIR(t->color));
        mvaddch(t->y + 1, t->x + 1, t->symbol);
        attroff(COLOR_PAIR(t->color));
}


// Draw list of things on stdscr
void draw_things(thing_list *things)
{
        while(things != NULL){
                draw_thing(things->thing);
                things = things->next;
        }
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

void place_in_world(world *w, thing *t, int x, int y){
        w->matrix[y*w->x + x] = t->symbol;
        t->x = x;
        t->y = y;
}

int main()
{
        int color = 2;
        int row, col;

        // Curses initstuff
        init_curses();
        getmaxyx(stdscr, row, col);

        // World init stuff
        world *w;
        w = create_world(col - 2, row - 2);
        init_world(w);

        // Player init stuff
        thing_list *head = create_thing_list(NULL);

        thing* player = init_thing(4, '@', 10, 10, head);
        init_thing(3, 'g', 20, 40, head);

        int c = 'n';
        while (c != 'q'){


                switch (c) {
                        case 's':
                                player->y += 1;
                                if(player->y > (w->y - 1)){
                                        player->y = w->y - 1;
                                }
                                break;
                        case 'w':
                                player->y -= 1;
                                if(player->y < 0){
                                        player->y = 0;
                                }
                                break;
                        case 'a':
                                player->x -= 1;
                                if(player->x < 0){
                                        player->x = 0;
                                }
                                break;
                        case 'd':
                                player->x += 1;
                                if(player->x > (w->x - 1)){
                                        player->x = w->x - 1;
                                }
                                break;
                        default: break;
                }
                draw_world(w, color); // Print world to screen
                draw_things(head);
                refresh();
                c = getch(); // Check if user input is available - non-blocking call
        }

        endwin();

        destroy_world(w);
        destroy_things(head);
        return 0;
}
