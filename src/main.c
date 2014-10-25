#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include<time.h>
#include<string.h>
#include<curses.h>
#include<unistd.h>
#include<omp.h>

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


// Print to stdsrc (curses)
void print_curses(world *w, int color)
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
        refresh();
}

// Initialize curses window
void init_curses()
{
        initscr();
        keypad(stdscr, true);
        noecho();
        cbreak();
        /* nodelay(stdscr, true); */
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
        init_pair(3, COLOR_BLUE, COLOR_BLACK);
        init_pair(4, COLOR_YELLOW, COLOR_BLACK);
}

int main()
{

        int color = 2;
        int row, col;

        world *w;
        init_curses();
        getmaxyx(stdscr, row, col);
        w = create_world(col - 2, row - 2);
        int c = 'n';
        while (c != 'q'){


                switch (c) {
                        default: break;
                }
                print_curses(w, color); // Print world to screen
                c = getch(); // Check if user input is available - non-blocking call
        }

        endwin();

        destroy_world(w);
        return 0;
}
