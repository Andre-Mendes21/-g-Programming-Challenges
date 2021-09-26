#ifndef BAG_H
#define BAG_H

#include <stdbool.h>

typedef struct bag_node_t
{
    void *data;
    struct bag_node_t *next;
} BagNode;

typedef struct bag_t
{
    BagNode *head;
    BagNode *tail;
    BagNode *current;
    int size;
} Bag;


/**
 * Create a new bag
 * 
 * @returns Pointer to a new, empty bag. If allocation fails return NULL
*/
Bag *bag_create(void);

/**
 * Destroy a bag
 * 
 * Frees the memory of the bag
*/
void bag_destroy(Bag *bag);

/**
 * Add an item to a bag
 * @param data must be dynamically allocated
*/
bool bag_add(Bag *bag, void *data);

/**
 * Get the number of items in a bag
*/
int size(Bag *bag);

/**
 * Iterates through the bag until current is NULL
*/
bool bag_iterate(Bag *bag);

/**
 * Print bag
*/
char **bag_print(Bag *bag);

#endif

