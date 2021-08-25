#include <stdio.h>
#include <stdlib.h>
#include "Bag.h"

BagNode *bag_node_create(void *item)
{
    BagNode *node = (BagNode *) malloc(sizeof(BagNode));

    if(node == NULL)
        return NULL;
    else
    {
        node->next = NULL;
        node->data = item;
    }

    return node;
}

Bag *bag_create(void)
{
    Bag *bag = (Bag *) malloc(sizeof(Bag));

    if(bag != NULL)
    {
        bag->head = NULL;
        bag->current = NULL;
        bag->size = 0;
    }

    return bag;
}

void bag_destroy(Bag *bag)
{
    BagNode *next_bag_node;
    bag->current = bag->head;

    while(bag->current != NULL)
    {
        next_bag_node = bag->current->next;
        free(bag->current->data);
        free(bag->current);
        bag->current = next_bag_node;
    }
    free(bag);
}

bool bag_add(Bag *bag, void *data)
{
    BagNode *node = bag_node_create(data);

    if(bag == NULL || node == NULL)
        return false;
    else
    {
        node->next = bag->head;
        bag->head = node;
    }

    return true;
}

int size(Bag *bag)
{
    return bag->size;
}

void bag_print(Bag *bag, void (*printfunc)(void *item))
{
    if(bag == NULL)
        return;
    
    else
    {
        printf("{");
        for(BagNode *node = bag->head; node != NULL; node = node->next)
        {
            if(printfunc != NULL)
                (*printfunc)(node->data);
            printf(", ");
        }
        printf("}");
    }
}