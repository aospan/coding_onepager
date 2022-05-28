// SPDX-License-Identifier: GPL-2.0-only
/*
 * Coding interview data structures and algos in plain C 
 *
 * Copyright (C) 2021 Abylay Ospan
 *
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Dear Programmer,
 * please update below counter and send me pull request on
 * https://github.com/aospan/coding_onepager
 */
#define TOTAL_PEOPLE_HELPED 1

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

/* Reverses arr[0..i] */
void flip(int arr[], int i)
{
	int temp, start = 0;
	while (start < i) {
		temp = arr[start];
		arr[start] = arr[i];
		arr[i] = temp;
		start++;
		i--;
	}
}

// Returns index of the
// maximum element in
// arr[0..n-1]
int find_max(int arr[], int n)
{
	int mi, i;
	for (mi = 0, i = 0; i < n; ++i)
		if (arr[i] > arr[mi])
			mi = i;
	return mi;
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
/*** STACK ***/
/*************/
/* Usage example:
	stack_t *st = alloc_stack();
	push(st, 42);
	if (!is_stack_empty(st))
		val = pop(st);
*/
typedef struct list_node {
	int val;
	struct list_node *next;
} list_node_t;

typedef struct {
	list_node_t *top;
} stack_t;

stack_t *alloc_stack()
{
	stack_t *st = malloc(sizeof(stack_t));
	st->top = NULL;
	return st;
}

void push(stack_t *st, int val)
{
	list_node_t *node = malloc(sizeof(list_node_t));
	node->val = val;
	node->next = st->top;
	st->top = node;
}

int pop(stack_t *st)
{
	// TODO: free allocated mem
	list_node_t *node = st->top;
	st->top = node->next;
	return node->val;
}

int is_stack_empty(stack_t *st)
{
	return st->top == NULL;
}


/*************/
/*** QUEUE ***/
/*************/
/*
         head                    tail
          |                        |
 NULL <- node <- node <- node <- node

 * enqueue to head
 * dequeue from tail

 Usage example:
	queue_t *q = alloc_queue();
  int val = 42;
	enqueue(q, &val);

  if (!is_empty(q)) {
		printf("queue size=%d\n", queue_size(q));
		printf("val=%d\n", *(int*)dequeue(q));
	}
*/
typedef struct queue_node {
	void *val;
	struct queue_node *prev;
} queue_node_t;

typedef struct {
	queue_node_t *head;
	queue_node_t *tail;
	int size;
} queue_t;

queue_t * alloc_queue()
{
	queue_t *q = malloc(sizeof(queue_t));
	q->head = NULL;
	q->tail = NULL;
	q->size = 0;
	return q;
}

void enqueue(queue_t *q, void *val)
{
	queue_node_t *node = malloc(sizeof(queue_node_t));
	node->val = val;
	node->prev = NULL;

	// add node to head & update head
	if (q->head)
		q->head->prev = node;
	q->head = node;

	// update tail if required
	if (!q->tail)
		q->tail = node;

	q->size++;
}

void * dequeue(queue_t *q)
{
	void *val = NULL;
	if (!q->tail) // attempt to dequeue from empty queue
		return NULL;

	// dequeue from tail & update tail
	queue_node_t *node = q->tail;
	q->tail = q->tail->prev;

	// update head if required
	if (!q->tail)
		q->head = NULL;

	q->size--; // dec queue size

	val = node->val;
	free(node);
	return val;
}

int queue_size(queue_t *q)
{
	return q->size;
}

int is_empty(queue_t *q)
{
	return q->tail == NULL;
}

/****************************/
/***        HASH TABLE    ***/
/****************************/
/* usage example:
	 hasht_t *ht = alloc_hasht(1024);
	 hasht_node_t *node = NULL;
	 hasht_add(ht, 42, 777);
	 node = hasht_search(ht, 42);
*/

typedef struct hasht_node {
	int key;
	int val;
	struct hasht_node * next;
} hasht_node_t;

typedef struct {
	int size;
	hasht_node_t **nodes;
} hasht_t;

// TODO: find better hash function
int hash_func (hasht_t *ht, int val)
{
	return abs(val) % ht->size;
}

hasht_t * alloc_hasht(int size) 
{
	hasht_t * ht = malloc(sizeof(hasht_t));
	ht->size = size;
	ht->nodes = malloc(size * sizeof(hasht_node_t*));
	memset(ht->nodes, 0, size * sizeof(hasht_node_t*));
	return ht;
}

hasht_node_t * hasht_alloc_node(int key, int val)
{
	hasht_node_t *node = malloc(sizeof(hasht_node_t));
	node->val = val;
	node->key = key;
	node->next = NULL;
	return node;
}

hasht_node_t * hasht_search(hasht_t *ht, int key)
{
	int hkey = hash_func(ht, key);
	hasht_node_t * node = ht->nodes[hkey];
	while (node) {
		if (node->key == key)
			return node;
		node = node->next;
	}
	return NULL;
}

void hasht_add (hasht_t *ht, int key, int val)
{
	int hkey = hash_func(ht, key);
	hasht_node_t *new_node = hasht_alloc_node(key, val);
	// check if we already have this key node
	hasht_node_t *node = hasht_search(ht, key);
	if (!node) {
		// new key, add it
		new_node->next = ht->nodes[hkey];
		ht->nodes[hkey] = new_node;
	}
}

/****************************/
/***  HASH TABLE v2       ***/
/***  key/val is void *   ***/
/****************************/
/* usage example:
	 hasht_v2_t *ht = alloc_hasht_v2(1024);
	 hasht_v2_node_t *node = NULL;
	 hasht_v2_add(ht, ptr_key, ptr_val);
	 node = hasht_v2_search(ht, 42);
*/

typedef struct hasht_v2_node {
	void *key;
	void *val;
	struct hasht_v2_node * next;
} hasht_v2_node_t;

typedef struct {
	int size;
	hasht_v2_node_t **nodes;
} hasht_v2_t;

// TODO: find better hash function
int hash_v2_func (hasht_v2_t *ht, void *i)
{
	// casting void* to int!
	return labs((intptr_t)i) % ht->size;
}

hasht_v2_t * alloc_hasht_v2(int size) 
{
	hasht_v2_t * ht = malloc(sizeof(hasht_v2_t));
	ht->size = size;
	ht->nodes = malloc(size * sizeof(hasht_v2_node_t*));
	memset(ht->nodes, 0, size * sizeof(hasht_v2_node_t*));
	return ht;
}

hasht_v2_node_t * hasht_v2_alloc_node(void *key, void *val)
{
	hasht_v2_node_t *node = malloc(sizeof(hasht_v2_node_t));
	node->val = val;
	node->key = key;
	node->next = NULL;
	return node;
}

hasht_v2_node_t * hasht_v2_search(hasht_v2_t *ht, void *key)
{
	int hkey = hash_v2_func(ht, key);
	hasht_v2_node_t * node = ht->nodes[hkey];
	while (node) {
		if (node->key == key)
			return node;
		node = node->next;
	}
	return NULL;
}

void hasht_v2_add (hasht_v2_t *ht, void *key, void *val)
{
	int hkey = hash_v2_func(ht, key);
	hasht_v2_node_t *new_node = hasht_v2_alloc_node(key, val);
	// check if we already have this key node
	hasht_v2_node_t *node = hasht_v2_search(ht, key);
	if (!node) {
		// new key, add it
		new_node->next = ht->nodes[hkey];
		ht->nodes[hkey] = new_node;
	}
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

/*** Pancake sort ***/
// source: https://www.geeksforgeeks.org/pancake-sorting/
void pancakeSort(int* arr, int n)
{
	// Start from the complete
	// array and one by one
	// reduce current size
	// by one
	for (int curr_size = n; curr_size > 1; --curr_size)
	{
		// Find index of the
		// maximum element in
		// arr[0..curr_size-1]
		int mi = find_max(arr, curr_size);

		// Move the maximum
		// element to end of
		// current array if
		// it's not already
		// at the end
		if (mi != curr_size - 1) {
			// To move at the end,
			// first move maximum
			// number to beginning
			flip(arr, mi);

			// Now move the maximum
			// number to end by
			// reversing current array
			flip(arr, curr_size - 1);
		}
	}
}

/*****************************/
/***         SEARCH        ***/
/*****************************/

// binary search
// recrusive impl below
// time complexity: O(log n)
// space complexity: O(log n) (stack memory)
// https://www.geeksforgeeks.org/binary-search/ (recursive + iterative implementations)
// https://en.wikipedia.org/wiki/Binary_search_algorithm
int binary_search(int arr[], int l, int r, int x)
{
	if (r >= l) {
		int mid = l + (r - l) / 2;

		// If the element is present at the middle
		// itself
		if (arr[mid] == x)
			return mid;

		// If element is smaller than mid, then
		// it can only be present in left subarray
		if (arr[mid] > x)
			return binary_search(arr, l, mid - 1, x);

		// Else the element can only be present
		// in right subarray
		return binary_search(arr, mid + 1, r, x);
	}

	// We reach here when element is not
	// present in array
	return -1;
}
