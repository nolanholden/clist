#ifndef _CLIST_H_
#define _CLIST_H_

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct cl_t cl_t;
typedef struct cl_node_t cl_node_t;

cl_t* cl_alloc_list();
size_t cl_size(const cl_t* l);
cl_node_t* cl_head(const cl_t* l);
cl_node_t* cl_tail(const cl_t* l);
cl_node_t* cl_alloc_node(void* data);
const cl_node_t* cl_next(const cl_node_t* n);
const cl_node_t* cl_prev(const cl_node_t* n);
void* cl_data(const cl_node_t* n);
void cl_destroy(cl_t* l);
void cl_destroy_node(cl_node_t* n);

// Push a node to the front. If the list is empty, node will become head and
// tail.
void cl_push_front(cl_t* l, cl_node_t* n);
void cl_push_back(cl_t* l, cl_node_t* n);
cl_node_t* cl_pop_back(cl_t* l);
cl_node_t* cl_pop_front(cl_t* l);

// Return whether the list is empty. Always O(1).
int cl_is_empty(const cl_t* l);

void cl_sort(cl_t* l, int(*greater_than)(void* first, void* second));

// Find a node in the list which matches the predicate. If predicate returns 0,
// no match; for return != 0, it is match.
cl_node_t* cl_find(int(*predicate)(const cl_node_t*), const cl_t* l);

#ifdef __cplusplus
}
#endif

#endif // _CLIST_H_
