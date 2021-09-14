#ifndef GRAPH_H
#define GRAPH_H

#include "Bag.h"

typedef struct graph_t
{
    int num_vertices;
    int num_edges;
    Bag **adj;
} Graph;

/**
 * Create a graph
 * 
 * @returns Pointer to a new, empty graph. If allocation fails returns NULL
*/ 
Graph *graph_create(int num_vertices);

/**
 * Create a graph and add the edges from a FILE
 * 
 * @returns Pointer to new, constructed graph. If allocation fails returns NULL
*/
Graph *graph_fill(FILE *input);

/**
 * Destroys a Graph
 * 
 * Frees the memory
*/
void graph_destroy(Graph *graph);

/**
 *  @returns Number of vertices
*/
int vertices(Graph *graph);

/**
 * @returns Number of edges
*/
int edges(Graph *graph);

/**
 * Adds an edge between two vertices
*/
void add_edge(Graph *graph, int v1, int v2);

/**
 * @returns The Adjanceny List of the Graph at position v
*/
Bag *adj(Graph *graph, int v);

/**
 * Prints a representation of the graph as an Adjacency List
*/
char **graph_print(Graph *graph);

#endif