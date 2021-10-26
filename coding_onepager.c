#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/***************/
/*** HELPERS ***/
/***************/
void print_array(int arr[], int n) {
  printf("array: ");
  for (int i = 0; i < n; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}

void swap (int *s, int *d) {
  int tmp = *s; 
  *s = *d; 
  *d = tmp;
}

/*************/
/*** QUEUE ***/
/*************/
typedef struct {
  int first;
  int last;
  int capacity;
  int size;
  int *array;
} queue_t;

queue_t * alloc_queue(int capacity) {
  queue_t *new_queue = malloc(sizeof(queue_t));
  new_queue->capacity = capacity;
  new_queue->array = malloc(capacity * sizeof(int));
  new_queue->last = -1; 
  new_queue->first = 0;
  new_queue->size = 0;
  return new_queue;
}

void enqueue(queue_t *q, int val) {
  q->last = (q->last + 1) % (q->capacity);
  q->array[q->last] = val;
  q->size++;
}

int dequeue(queue_t *q) {
  int val = q->array[q->first];
  q->first = (q->first + 1) % (q->capacity);
  q->size--;
  return val;
}

bool is_empty(queue_t *q) {
  return q->size ? false:true;
}

/****************************/
/***        GRAPH         ***/
/*** adjacency list graph ***/
/****************************/
typedef struct node {
  struct node *next;
  int vertex_id;
} node_t;

typedef struct graph {
  int vertices_num;
  node_t **vertices;
} graph_t;

node_t * alloc_node(int vertex_id) {
  node_t * new_vertex = malloc(sizeof(node_t));
  new_vertex->vertex_id = vertex_id;
  return new_vertex;
}

graph_t * alloc_graph(int vertices_num) {
  graph_t * gr = malloc(sizeof(graph_t));
  gr->vertices_num = vertices_num;
  gr->vertices = malloc(vertices_num * sizeof(node_t));

  // initialize
  for(int i = 0; i < vertices_num; i++)
    gr->vertices[i] = NULL;

  return gr;
}

void add_edge(graph_t *gr, int src, int dst) {
  node_t *new_node;

  // add src->dst edge
  new_node = alloc_node(dst);
  new_node->next = gr->vertices[src];
  gr->vertices[src] = new_node;

  // add dst->src edge
  new_node = alloc_node(src);
  new_node->next = gr->vertices[dst];
  gr->vertices[dst] = new_node;
}

void print_graph (graph_t *gr) {
  node_t * node;

  for (int i = 0; i < gr->vertices_num; i++) {
    node = gr->vertices[i];
    printf("vertex %d:\n", i);

    while (node) {
      printf(" dst:%d\n", node->vertex_id);
      node = node->next;
    }
  }
}
