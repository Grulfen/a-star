#ifndef THING_UTIL_H
#define THING_UTIL_H
#include<stdlib.h>

// Struct for thing in world
typedef struct {
        int color;
        char symbol;
        int x;
        int y;
} thing;

// FIXME (kanske bättre med array?)
// Struct for list of things
typedef struct node {
        thing *thing;
        struct node *next;
} thing_list;

thing_list *create_thing_list(thing* t);

// Add a thing to the end of the list
thing_list *add_thing(thing_list* head, thing* t);

// return size of thing_list
int thing_list_size(thing_list *head);

void destroy_things(thing_list *head);

thing *init_thing(int color, char symbol, int x, int y, thing_list *list);

#endif
