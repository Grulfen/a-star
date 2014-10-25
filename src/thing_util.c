#include "thing_util.h"

thing_list *create_thing_list(thing* t)
{
        thing_list *list = malloc(sizeof(thing_list));
        list->next = NULL;
        list->thing = t;
        
        return list;
}

// Add a thing to the end of the list
thing_list *add_thing(thing_list* head, thing* t)
{
        thing_list *new_item = malloc(sizeof(thing_list));
        new_item->next = NULL;
        new_item->thing = t;
        thing_list *tmp = head;
        while(tmp->next != NULL){
                tmp = tmp->next;
        }
        tmp->next = new_item;
        return new_item;
}

// return size of thing_list
int thing_list_size(thing_list *head)
{
        int c = 1;
        while(head->next != NULL){
                c++;
                head = head->next;
        }
        return c;
}

void destroy_things(thing_list *head)
{
        if(head == NULL){
                return;
        }

        thing_list *next = head->next;
        free(head->thing);
        free(head);
        destroy_things(next);
}

// Create a thing and add it to thing_list "list"
thing *init_thing(int color, char symbol, int x, int y, thing_list *list)
{
        thing *player = malloc(sizeof(thing));
        player->color = color;
        player->symbol = symbol;
        player->x = x;
        player->y = y;
        add_thing(list, player);
        return player;
}
