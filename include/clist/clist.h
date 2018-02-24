#ifndef _CLIST_H_
#define _CLIST_H_

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct cl_t cl_t;
typedef struct cl_node_t cl_node_t;

// Dynamically allocate a new, empty list. Returns NULL on failure to allocate.
cl_t* cl_alloc_list();

// Get the size of the list.
size_t cl_size(const cl_t* l);

// Return the node at the front, without popping.
const cl_node_t* cl_peak_front(const cl_t* l);

// Return the node at the back, without popping.
const cl_node_t* cl_peak_back(const cl_t* l);

// Dynamically allocate a new node with the underlying data provided. Returns 
// NULL on failure to allocate.
cl_node_t* cl_alloc_node(void* data);

// Get the next node to the given node. NULL if the given node is the back.
const cl_node_t* cl_next(const cl_node_t* n);

// Get the previous node to the given node. NULL if the given node is the front.
const cl_node_t* cl_prev(const cl_node_t* n);

// Fetch the data at the given node.
void* cl_data(const cl_node_t* n);

// Destroy the list and all of its nodes, applying the destroy_data function 
// to each node's underlying data. If the function is NULL, the data is left 
// malloc'd.
void cl_destroy(cl_t* l, void(*destroy_data)(void* data));

// Destroy the given node, applying the destroy_data function the node's 
// underlying data. If the function is NULL, the data is left malloc'd.
void cl_destroy_node(cl_node_t* n, void(*destroy_data)(void* data));

// For each item in the list, apply the use_data function to the underlying 
// data.
void cl_foreach(cl_t* l, void(*use_data)(void* data));

// Push a node to the front. If the list is empty, node will become head and
// tail.
void cl_push_front(cl_t* l, cl_node_t* n);

// Push a node to the back. If the list is empty, node will become head and
// tail.
void cl_push_back(cl_t* l, cl_node_t* n);

// Push data to the front, automatically malloc'ing a containing node. If the 
// list is empty, node will become head and tail.
void cl_empl_front(cl_t* l, void* data);

// Push data to the back, automatically malloc'ing a containing node. If the 
// list is empty, node will become head and tail.
void cl_empl_back(cl_t* l, void* data);

// Pop the node at the back. The node and underlying data remain malloc'd.
cl_node_t* cl_pop_back(cl_t* l);

// Pop the data at the back, automatically detroying the containing node.
void* cl_popd_back(cl_t* l);

// Pop the node at the front. The node and underlying data remain malloc'd.
cl_node_t* cl_pop_front(cl_t* l);

// Pop the data at the front, automatically detroying the containing node.
void* cl_popd_front(cl_t* l);

// Return whether the list is empty. Always O(1).
int cl_is_empty(const cl_t* l);

// Sort the list based on the provided comparator function.
void cl_sort(cl_t* l, int(*greater_than)(const void* first, const void* second));

// Find a node in the list which matches the predicate. If predicate returns 0,
// no match; for return != 0, it is match.
cl_node_t* cl_find(const cl_t* l, int(*predicate)(const void* data));

#ifdef __cplusplus
}
#endif

#endif // _CLIST_H_
