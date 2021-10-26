/* SOURCE:
 * Breadth First Search: Shortest Reach
 * https://www.hackerrank.com/challenges/bfsshortreach/problem
 */

/*
 * Complete the 'bfs' function below.
 *
 * The function is expected to return an INTEGER_ARRAY.
 * The function accepts following parameters:
 *  1. INTEGER n
 *  2. INTEGER m
 *  3. 2D_INTEGER_ARRAY edges
 *  4. INTEGER s
 */

/*
 * To return the integer array from the function, you should:
 *     - Store the size of the array to be returned in the result_count variable
 *     - Allocate the array statically or dynamically
 *
 * For example,
 * int* return_integer_array_using_static_allocation(int* result_count) {
 *     *result_count = 5;
 *
 *     static int a[5] = {1, 2, 3, 4, 5};
 *
 *     return a;
 * }
 *
 * int* return_integer_array_using_dynamic_allocation(int* result_count) {
 *     *result_count = 5;
 *
 *     int *a = malloc(5 * sizeof(int));
 *
 *     for (int i = 0; i < 5; i++) {
 *         *(a + i) = i + 1;
 *     }
 *
 *     return a;
 * }
 *
 */
int* bfs(int n, int m, int edges_rows, int edges_columns, int** edges, int s, int* result_count) {
    queue_t *q = alloc_queue(n);
    int distance[n];
    node_t * vertex;
    int vertex_id = 0;
    int *res, res_count = 0;

    res = malloc(n * sizeof(int));
    memset(res, 0, n * sizeof(int));
    
    // init distance
    for (int i = 0; i < n; i++) {
        distance[i] = -1;
    }

    printf("vertices=%d edges=%d edges_rows=%d edges_columns=%d s=%d\n",
    n, m, edges_rows, edges_columns, s - 1);
    
    // alloc graph and add edges
    graph_t *gr = alloc_graph(n);
    for (int i = 0; i < edges_rows; i++) {
        add_edge(gr, edges[i][0]-1, edges[i][1]-1);
        printf ("  adding edge%d = %d->%d\n", i, edges[i][0]-1, edges[i][1]-1);
    }
    print_graph(gr);
    
    enqueue(q, s - 1); // enqueue starting point (vertex)
    distance[s - 1] = 0;

    while (!is_empty(q)) {
        vertex_id = dequeue(q);
        vertex = gr->vertices[vertex_id];

        // visit here
        printf("visiting: %d distance: %d\n", vertex_id, distance[vertex_id]);

        // enqueue childs
        while (vertex) {
            if (distance[vertex->vertex_id] == -1) {
              distance[vertex->vertex_id] = distance[vertex_id] + 1;
              enqueue(q, vertex->vertex_id);
            }

            // printf("enqueue child: %d distance: %d\n", vertex->vertex_id, distance[vertex->vertex_id]);
            vertex = vertex->next;
        }
    }

    printf("distances:");
    for (int i = 0; i < n; i++) {
        printf("final distance %d -> %d \n", i, (distance[i] < 0)?distance[i]:(distance[i]*6));

        if (i == s - 1)
            continue;
            
        res[res_count] = (distance[i] < 0)?distance[i]:(distance[i]*6);
        res_count++;
    }
    
    *result_count = res_count;
    return res;
}
