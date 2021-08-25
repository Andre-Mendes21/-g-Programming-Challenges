#ifndef BAG_H
#define BAG_H

#include <stdbool.h>

typedef struct BagNode
{
    void *data;
    struct bagnode *next;
} BagNode;

typedef struct Bag
{
    BagNode *head;
    BagNode *current;
    int size;
} Bag;

/**
 * Create a new node to store the given item
 * 
*/
BagNode *bag_node_create(void *item);

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
*/
bool bag_add_item(Bag *bag, void *data);

/**
 * Get the number of items in a bag
*/
int size(Bag *bag);

/**
 * Print bag
*/
void bag_print(Bag *bag, void (*printfunc)(void *item));

#endif

