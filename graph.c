
#include <stdio.h>
#include "graph.h"
#include <stdlib.h>
#include <string.h>


/*
  This program is my implemention of a directed graph.
  The graph structure contains a dynamically allocated array
  of vertices, and each vertex has a list of edges and a string
  representing its name.
 */


/*
  This function initializes the graph paremeter. 
  As a result, the number of vertices is set to 0 and the
  beginning vertex set to NULL.
  All structures are dynamically-allocated.
*/
void init_graph ( Graph *graph ) {
	
  if ( graph == NULL )
    return;
  /* Initialize the initial vertex and edge list */
  graph->v_list = malloc( sizeof( Vertex ) );
  
  /* Set number of vertices to 0 */
  graph->num_v = 0;
}


/* 
   This function ads a new vertex with name new_vertex to its 
   parameter graph. If there is already a vertex with present
   with the same name, the function will return 0 without
   changing anything. Otherwise 1 will be returned and the
   vertex will be added to the graph.
*/

int add_vertex ( Graph *graph, const char new_vertex[] ) {
  char *v_name;
  Vertex *cur;
  /* Check if any parameters are NULL */
  if ( graph == NULL || new_vertex == NULL )
    return 0;
  /* Check if vertex is already in graph */
  if ( has_vertex( *graph, new_vertex ) )
    return 0;

  v_name = malloc( strlen(new_vertex) + 1);
  strcpy( v_name, new_vertex );
  /* Case empty graph */
  if ( graph->num_v == 0 ) {
		
    graph->v_list->name = v_name;
    graph->v_list->next_v = NULL;
    graph->v_list->edge_list = NULL;

    graph->num_v++;
  } else { /* Else add another vertex to existing linked list */
    cur = graph->v_list;
    while ( cur->next_v != NULL )
      cur = cur->next_v;
    
    cur->next_v = malloc ( sizeof ( Vertex ) );
    cur->next_v->name = v_name;
    cur->next_v->next_v = NULL;
    cur->next_v->edge_list = NULL;

    graph->num_v++;
  }
  return 1;

}

/* 
   This function returns 1 if its parameter graph already
   contains a vertex with the name parameter, and 0 if not.
*/
int has_vertex ( Graph graph, const char name[] ) {
  
  Vertex *cur;
  if ( name == NULL )
    return 0;
  if ( graph.num_v == 0 )
    return 0;
  cur = graph.v_list;

  while ( cur != NULL ) {
    if ( strcmp(cur->name, name) == 0 )
      return 1;
    cur = cur->next_v;
  }
  return 0;
}


/*
  This function returns the number of vertices in the grah.
  The value will always be zero or greater. 
*/
int num_vertices ( Graph graph ) {
  return (graph.num_v);
}

/* 
   This function adds a new edge to the graph that goes from
   the vertex source to the vertex dest, and has the cost specified.
   IF the edge is added successfully 1 is returned. If the edge is
   not added then nothing is modified and 0 is returned.

   Edge is not added when :
   Any pointer or array parameters are NULL
   Cost is negative.
   Source or dest does not exist in the graph.
   Already existing edge from source to dest.
*/
int add_edge ( Graph *graph, const char source[],
	       const char dest[], int cost ) {

  Vertex *src, *destination, *cur;
  Edge *current;
  

  /* Cases where edge is not added */
  if ( graph == NULL || source == NULL || dest == NULL )
    return 0;
  if ( cost < 0 )
    return 0;
  if ( !has_vertex(*graph, source) || !has_vertex(*graph, dest) )
    return 0;
  if ( get_edge_cost( *graph, source, dest) != -1 )
    return 0;

  /* Go through all vertices to look for source and destination */
  cur = graph->v_list;
  while ( cur != NULL ) {
    if ( strcmp(source, cur->name) == 0 )
      src = cur;
    if ( strcmp(dest, cur->name) == 0 )
      destination = cur;
    cur = cur->next_v;
  }

  /* Dynamically allocate memory to edge if it's still NULL */
  if ( src->edge_list == NULL ) {
    src->edge_list = malloc (sizeof (Edge) );

    /* ALlocate the next edge and destination vertex and cost */
    src->edge_list->next_edge = NULL;
    src->edge_list->dest_v = destination;
    src->edge_list->cost = cost;
    return 1;
  } else { /* Add to end of edge list */
    current = src->edge_list;
    while ( current->next_edge != NULL ) /* Loop until end of edge list */
      current = current->next_edge;

    /* Initialize and allocate fields of the new edge */
    current->next_edge = malloc( sizeof (Edge) );
    current->next_edge->dest_v = destination;
    current->next_edge->cost = cost;
    current->next_edge->next_edge = NULL;

    return 1;
  }

  return 0;

}

/* 
   This function returns the cost of the edge in the graph from the 
   vertex source to the vertex destination;
   -1 is returned if source or dest is not in the graph or if 
   there is no edge between them 
*/
int get_edge_cost ( Graph graph, const char source[], const char dest[] ) {
  Vertex *src, *cur;
  Edge *current;
  

  if ( source == NULL || dest == NULL )
    return -1;
  /* Check if both vertices exist in the graph */
  if ( !has_vertex(graph, source) || !has_vertex(graph, dest) )
    return -1;

  /* Search to see if there is an edge between source and dest */
  cur = graph.v_list;
  while ( cur != NULL ) {
    if ( strcmp( source, cur->name ) == 0 )
      src = cur;
    cur = cur->next_v;
  }

  /* Loop through all edges in the list to see if one exists between
     source and dest */
  current = src->edge_list;
  while ( current != NULL ) {
    if ( strcmp(dest, current->dest_v->name) == 0 )
      return current->cost;

    current = current->next_edge;
  }

  return -1; /* Edge not found */
}

/* 
   This function modifies the cost of the edge in the graph between
   source and dest to be new_cost, returning 1 if successful.
   It will be unsuccessful and return 0 without changing anything if:
   new_cost is negative 
   source and dest are NULL or not vertices in the graph
   source and dest are in graph but have no edge between them
*/
int change_edge_cost ( Graph *graph, const char source[], const char dest[],
		       int new_cost ) {
  
  Vertex *src, *cur;
  Edge *current;

  /* Cases where this function should not do anything */
  if ( graph == NULL || source == NULL || dest == NULL )
    return 0;
  if ( new_cost < 0 )
    return 0;
  if ( !has_vertex(*graph, source) || !has_vertex(*graph, dest) )
    return 0;
  if ( get_edge_cost(*graph, source, dest) == -1 )
    return 0;

  /* Search for source vertex */
  cur = graph->v_list;
  while ( cur != NULL ) {
    if ( strcmp(source, cur->name) == 0 )
      src = cur;
    cur = cur->next_v;
  }

  /* Loop through all edges in the list to see if one exists
     between source and dest, then change the cost */
  current = src->edge_list;
  while ( current != NULL ) {
    if ( strcmp(dest, current->dest_v->name ) == 0)
      current->cost = new_cost; /* Edge found, change cost */

    current = current->next_edge;
  }

  return 1;

}
/* 
   This functions returns the number of neighbors that its parameter
   vertex has in its parameter graph. If there is no vertex with that name 
   in the graph, -1 is returned. If the array parameter is NULL,
   -1 is returned;
*/
int num_neighbors ( Graph graph, const char vertex[] ) {
  Edge *current;
  int neighbors = 0;
  Vertex *v, *cur;
 

  /* Cases where function does not do anything */
  if ( !has_vertex( graph, vertex) ) 
    return -1;
  if ( vertex == NULL )
    return -1;

  /* Locate vertex in question */
  cur = graph.v_list;
  while ( cur != NULL ) {
    if ( strcmp(vertex, cur->name) == 0 )
      v = cur;
    cur = cur->next_v;
  }

  /* Vertex found, begin to count number of edges in edge list */
  if ( v->edge_list == NULL )
    return 0;
  current = v->edge_list;
  while ( current != NULL ) {
    neighbors++;
    current = current->next_edge;
  }

  return neighbors;
}	


/*
  This function will deallocate any dynamically-allocated memory that
  is used by the entire Graph variable that its parameter graph points to.
*/

void clear_graph ( Graph *graph ) {

  Vertex *cur, *prev;
  Edge *cur_e, *tmp;
  if ( graph == NULL )
    return;
  cur = graph->v_list;
  while ( cur != NULL ) {
    prev = cur;
    cur = cur->next_v;

    tmp = prev->edge_list;
    while ( tmp != NULL ) {
      cur_e = tmp;
      tmp = tmp->next_edge;
      free ( cur_e );
    }
    free ( prev->name );
    free ( prev );
  }
  graph->num_v = 0;
}


/* 
   This function will return a newly-created dynamically allocated
   array of pointers to strings, where each pointer is the name of
   the vertices in the graph. The vertex names will be in increasing
   lexicographic order in the array. It will make copies of the vertex
   names to store in the returned array.

   If there are n vertices in the graph, the function will return an
   array with n + 1 elements, where the last element is NULL.
*/
char **get_vertices ( Graph graph ) {
  Vertex *cur;
  char **vertices;
  char *name_copy;
  int i, j;

  /* If there are no vertices in the graph, array of one element
     (NULL) is returned */
  if ( graph.num_v == 0 ) {
    vertices = malloc ( sizeof ( char * ) );
    vertices[0] = NULL;
  } else {
    /* Initialize the vertex name array to be returned */
    vertices = malloc ( (graph.num_v + 1) * sizeof( char * ) );

    /* Store all vertices in the graph into the array */
    cur = graph.v_list;
    i = 0;
    while ( cur != NULL ) {
      vertices[i] = malloc ( strlen(cur->name) + 1 );
      strcpy( vertices[i], cur->name );
      cur = cur->next_v;
      i++;
    }

    /* Sort the string array */
    for ( i = 0; i < (graph.num_v); i++ ) {
      for ( j = 0; j < (graph.num_v); j++ ) {
	if ( strcmp( vertices[i], vertices[j] ) < 0 ) {
	  name_copy = malloc( strlen(vertices[i]) + 1 );
	  strcpy( name_copy, vertices[i] );

	  free( vertices[i] );
	  vertices[i] = malloc( strlen(vertices[j]) + 1 );
	  strcpy( vertices[i], vertices[j] );

	  free( vertices[j] );
	  vertices[j] = malloc( strlen(name_copy) + 1 );
	  strcpy( vertices[j], name_copy );

	  free( name_copy );
	}
      }
    }
		    
		    
    vertices[(graph.num_v)] = NULL;
  }
  return vertices;
}
/*
  This method returns a newly-created dynamically allocated
  array of pointers to strings that are all neighbors of the
  parameter vertex. It will be in increasing lexicographic order.

  If the parameter passed in is not in the graph, NULL is returned.
*/
char **get_neighbors( Graph graph, const char vertex[] ) {

  Vertex *src, *cur;
  Edge *current;
  int i, j;
  char **neighbors;
  char *name_copy;
  if (!has_vertex( graph, vertex ) )
    return NULL;
  if ( vertex == NULL )
    return NULL;
  /* Search for source vertex */
  cur = graph.v_list;
  while ( cur != NULL ) {
    if ( strcmp(vertex, cur->name) == 0 )
      src = cur;
    cur = cur->next_v;
  }

  /* No neighbors case */
  if ( src->edge_list == NULL ) {
    neighbors = malloc( sizeof( char * ) );
    neighbors[0] = NULL;
    return neighbors;
  }
  
  neighbors = malloc( ((num_neighbors(graph, vertex) + 1 ) )
		       * sizeof(char *)  );
 
  i = 0;
  current = src->edge_list;

  while ( current != NULL ) {
    neighbors[i] = malloc ( (strlen(current->dest_v->name) + 1 )
			    * sizeof( char ));
    strcpy ( neighbors[i], current->dest_v->name );
    i++;
    current = current->next_edge;
  }

  
  
  /* Sort the neighbors array */
  for ( i = 0; i < num_neighbors(graph, vertex); i++ ) {
    for ( j = 0; j < num_neighbors(graph, vertex); j++ ) {
        if ( strcmp( neighbors[i], neighbors[j] ) < 0 ) {
	  name_copy = malloc( strlen(neighbors[i]) + 1 );
	  strcpy( name_copy, neighbors[i] );

	  free( neighbors[i] );
	  neighbors[i] = malloc( strlen(neighbors[j]) + 1 );
	  strcpy( neighbors[i], neighbors[j] );

	  free( neighbors[j] );
	  neighbors[j] = malloc( strlen(name_copy) + 1 );
	  strcpy( neighbors[j], name_copy );

	  free( name_copy );
	}
      }
  }
  neighbors[ num_neighbors(graph, vertex) ] = NULL;
  
  return neighbors;
}

/**
   This function frees all of the dynamically
   allocated memory of the strings in the array parameter.
   If the pointer passed in is NULL, there is no effect.
**/

void free_vertex_name_list ( char **vertex_names ) {
  int i = 0;
  char *free_name;
  char **holder = vertex_names;

  if ( vertex_names == NULL )
    return;

  free_name = vertex_names[0];

  while ( free_name != NULL ) {
    free_name = vertex_names[++i];
    free( vertex_names[ i - 1 ] );
  }

  free( holder );

}

/*
  This function removes the edge going from its two parameters.
  If either vertex is not in the graph, or if there is no
  edge between them, then function will return 0. 
  Otherwise, upon successful removal 1 will be returned.
  Any memory no longer in use is freed.
*/
int remove_edge ( Graph *graph, const char source[], const char dest[] ) {
  
  Vertex *src, *cur;
  Edge *prev, *current;

  if ( !has_vertex(*graph, source) || !has_vertex(*graph, dest) )
    return 0;
  if ( get_edge_cost( *graph, source, dest ) == -1 )
    return 0;
  if ( graph == NULL || source == NULL || dest == NULL )
    return 0;
  /* Find the source vertex */
  cur = graph->v_list;
  while ( cur != NULL ) {
    if ( strcmp(source, cur->name) == 0 )
      src = cur;
    cur = cur->next_v;
  }
  

  /* Find the edge to be removed */
  current = src->edge_list;
  prev = NULL;
  while ( strcmp( current->dest_v->name, dest ) != 0 && current != NULL) {
    prev = current;
    current = current->next_edge;
  }

  /* Remove current edge */
  if ( prev == NULL ) {
    src->edge_list = current->next_edge;
    free ( current );
  } else {
    prev->next_edge = current->next_edge;
    free ( current );

  }
  

  return 1;
}

/* 
   This function removes the vertex parameter from the graph passed in.
   If it is not contained in the graph, 0 is returned.
   Otherwise, the vertex is removed and 1 is returned. 
   Any memory no longer in use is removed.
*/
int remove_vertex ( Graph *graph, const char vertex[] ) {
  
  Vertex *src, *cur, *prev;
  Edge *current, *next_edge;

  if ( graph == NULL || vertex == NULL )
    return 0;
  cur = graph->v_list;
  while ( cur != NULL ) {
    if ( strcmp(cur->name, vertex) != 0 ) {
      current = cur->edge_list;

      /* Find all INCOMING edges and free them */
      while ( current != NULL ) {
	if ( strcmp( current->dest_v->name, vertex ) == 0 ) {
	  next_edge = current->next_edge;
	  remove_edge( graph, cur->name, vertex );
	  current = next_edge;
	} else  current = current->next_edge;
      }
    }
    cur = cur->next_v;
  }
  /* Free any outgoing edges */
  cur = graph->v_list;
  
  while ( cur != NULL ) {
    if ( strcmp( cur->name, vertex ) == 0 )
      src = cur;
    cur = cur->next_v;
  }
  
 

  /* Remove all outgoing edges in the edge list */
  current = src->edge_list;
  while ( current != NULL ) {
    next_edge = current->next_edge;
    remove_edge( graph, vertex, current->dest_v->name );
    current = next_edge;
  }

  /* Finally remove the vertex */
  
  cur = graph->v_list;
  prev = NULL;
  while ( strcmp( cur->name, vertex ) != 0 ) {
    prev = cur;
    cur = cur->next_v;
  }
  if ( prev == NULL ) {
    graph->v_list = cur->next_v;
    free ( cur->name );
    free ( cur );
  } else {
    prev->next_v = cur->next_v;
    free ( cur->name );
    free ( cur );
  }
  
  
  
  graph->num_v--;
  
  
  return 1;
}
