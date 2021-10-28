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

// from http://www.geeksforgeeks.org/write-a-c-program-to-calculate-powxn/
float power(float x, int y)
{
  float temp;
  if( y == 0)
    return 1;
  temp = power(x, y/2);       
  if (y%2 == 0)
    return temp*temp;
  else
  {
    if(y > 0)
      return x*temp*temp;
    else
      return (temp*temp)/x;
  }
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

/*****************************/
/***         SORTING       ***/
/*****************************/

/*** Insertion Sort ***/
// https://www.hackerrank.com/challenges/insertionsort2/problem
// https://en.wikipedia.org/wiki/Insertion_sort
void insertion_sort(int n, int arr_count, int* arr)
{
  for (int i = 1; i < n; i++) {
    for (int j = i; j > 0 && arr[j-1] > arr[j]; j--) {
      if (arr[j] < arr[j-1])
        swap(&arr[j-1], &arr[j]);
    }
    print_array(arr, n);
  }
}

/*** Quick sort ***/
// https://en.wikipedia.org/wiki/Quicksort
int partition(int l, int r, int *ar)
{
  int pivot = ar[r]; // last element as pivot
  int pivot_index = l;

  for (int i = l; i < r; i++) {
    if (ar[i] < pivot) {
      swap(&ar[i], &ar[pivot_index]);
      pivot_index++;
    }
  }

  // place pivot val to found pivot index
  swap(&ar[r], &ar[pivot_index]);

  return pivot_index;
}

// usage:
// quicksort(0, ar_size - 1, ar);
void quicksort(int l, int r, int *ar)
{
	int pivot_index;

	if (l >= r)
		return;

	pivot_index = partition(l, r, ar);

	quicksort(l, pivot_index -1, ar); // left side
	quicksort(pivot_index + 1, r, ar); // right part
}

/*** Heap sort ***/
// https://en.wikipedia.org/wiki/Heapsort
void heapify(int parent, int ar_size, int *ar)
{
  int largest = parent;
  int l = parent*2 + 1;
  int r = parent*2 + 2;

  if (l < ar_size && ar[l] > ar[largest])
    largest = l;

  if (r < ar_size && ar[r] > ar[largest])
    largest = r;

  if (largest != parent) {
    swap (&ar[largest], &ar[parent]);

    // fix affected sub-heap
    heapify(largest, ar_size, ar);
  }
}

// usage:
// int ar[8] = {1, 3, 2, 4, 7, 9, 6, 5};
// int ar_size = sizeof(ar)/sizeof(ar[0]);
// heapsort(ar_size, ar);
void heapsort(int ar_size, int *ar)
{
  for (int i = ar_size/2 - 1; i >= 0; i--) {
    heapify(i, ar_size, ar);
  }

  // shrink array by 1 and fix resulting heap
  for (int i = ar_size - 1; i > 0; i--) {
    // swap first <-> last
    swap(&ar[0], &ar[i]);

    // fix heap
    heapify(0, i, ar);
  }
}
