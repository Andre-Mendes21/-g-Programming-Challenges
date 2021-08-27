#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Bag.h"

BagNode *bag_node_create(void *item)
{
    BagNode *node = malloc(sizeof(BagNode));

    if(node == NULL)
    {
        return NULL;
    }

    else
    {
        node->data = item;
        node->next = NULL;
    }

    return node;
}

Bag *bag_create(void)
{
    Bag *bag = malloc(sizeof(Bag));

    if(bag != NULL)
    {
        bag->head = NULL;
        bag->tail = NULL;
        bag->current = NULL;
        bag->size = 0;
    }

    return bag;
}

void bag_destroy(Bag *bag)
{
    BagNode *next_bag_node;
    bag->current = bag->head;
    unsigned i = 1;
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
    BagNode *new_node = bag_node_create(data);

    if(bag == NULL || new_node == NULL)
        return false;

    if(bag->size == 0)
    {
        bag->head = new_node;
        bag->tail = new_node;
        bag->current = bag->head;
    }

    else
    {
        bag->tail->next = new_node;
        bag->tail = new_node;
    }
    bag->size++;

    return true;
}

int size(Bag *bag)
{
    return bag->size;
}

bool bag_iterate(Bag *bag)
{
    if(bag->current == NULL || bag->current->next == NULL)
    {
        return false;
    }

    else
    {
        bag->current = bag->current->next;
        return true;
    }
}

char **bag_print(Bag *bag)
{
    char **output;

    if(bag == NULL)
    {
        return NULL;
    }

    else
    {
        bag->current = bag->head;

        char *string = (char *) bag->current->data;
        output = malloc(sizeof(char *) * bag->size);

        output[0] = malloc(strlen(string) + 1);
        strcpy(output[0], string);

        unsigned i = 1;
        while(bag_iterate(bag))
        {
            string = (char *) bag->current->data;
            output[i] = malloc(strlen(string) + 1);
            strcpy(output[i++], string);
        }
    }

    return output;
}