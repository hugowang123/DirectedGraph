#if !defined(GRAPH_IMPLEMENTATION_H)
#define GRAPH_IMPLEMENTATION_H

struct vertex; /* Partial Declaration */

typedef struct edge {
  struct vertex *dest_v;
  struct edge *next_edge;
  int cost;
} Edge;

typedef struct vertex {
  Edge *edge_list;
  struct vertex *next_v;
  char *name;
} Vertex;

typedef struct graph {
  int num_v;
  Vertex *v_list;
} Graph;

#endif

		       
