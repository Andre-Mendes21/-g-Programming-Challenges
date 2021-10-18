#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "Graph.h"
#include "Bag.h"

Graph *graph_create(int num_vertices)
{
    Graph *graph = malloc(sizeof(Graph));

    if(graph != NULL)
    {
        graph->num_vertices = num_vertices;
        graph->num_edges = 0;
        graph->adj = malloc(num_vertices * sizeof(Bag));

        for(int v = 0; v < num_vertices; v++)
        {
            graph->adj[v] = bag_create();
        }
    }

    return graph;
}

Graph *graph_fill(FILE *input)
{
    Graph *graph;

    char temp_buff[8];
    if(fgets(temp_buff, sizeof(temp_buff), input) == NULL)  // Reading number of vertices
    {                                                       // from file
        return NULL;
    }

    int num_vertices = atoi(temp_buff);
    graph = graph_create(num_vertices);

    fgets(temp_buff, sizeof(temp_buff), input);             // Reading number of edges
    int num_edges = atoi(temp_buff);                        // from file

    for(int i = 0; i < num_edges; i++)
    {
        fgets(temp_buff, sizeof(temp_buff), input);

        char v_char = temp_buff[0];
        char w_char = temp_buff[2];

        if(isdigit(v_char) && isdigit(w_char))
        {
            int v = v_char - '0';
            int w = w_char - '0';
            
            add_edge(graph, v, w);
        }
    }

    return graph;
}

void graph_destroy(Graph *graph)
{
    for(int i = 0; i < graph->num_vertices; i++)
    {
        bag_destroy(graph->adj[i]);
    }

    free(graph->adj);
    free(graph);
}

int vertices(Graph *graph)
{
    return graph->num_vertices;
}

int edges(Graph *graph)
{
    return graph->num_edges;
}

void add_edge(Graph *graph, int v, int w)
{
    if(v >= graph->num_vertices || w >= graph->num_vertices)
    {
        printf("ERROR: Vertex Pair: (%d, %d) must be within vertex limit of %d\n", v, w, graph->num_vertices);
        exit(1);
    }

    int *v_2 = (int *) malloc(1 * sizeof(w));
    int *w_2 = (int *) malloc(1 * sizeof(v));

    *v_2 = v;
    *w_2 = w;

    bag_add(graph->adj[v], w_2);
    bag_add(graph->adj[w], v_2);

    graph->num_edges++;
}

Bag *graph_adj(Graph *graph, int v)
{
    return graph->adj[v];
}

char **graph_print(Graph *graph)
{
    char **output;

    if(graph == NULL)
    {
        return NULL;
    }

    else
    {
        char string[1024];
        output = malloc(graph->num_vertices * sizeof(char *));

        for(int i = 0; i < graph->num_vertices; i++)
        {
            Bag *adj = graph_adj(graph, i);
            sprintf(string, "%d:", i);

            for(int j = 0; j < adj->size; j++)
            {
                char *bag = bag_print(adj)[j];
                char temp[2]; 
                sprintf(temp, "%d", *(int *) bag);

                strcat(string, " ");
                strcat(string, temp);
            }

            output[i] = malloc(strlen(string + 1) * sizeof(char));
            strcpy(output[i], string);
        }
    }
    return output;
}