#ifndef _CLIST_H_
#define _CLIST_H_

#include <stddef.h>
#include <stdint.h>

typedef struct cl_t cl_t;
typedef struct cl_node_t cl_node_t;

cl_t* cl_alloc_list();
size_t cl_size(const cl_t* l);

// cl_node_t* cl_alloc_node(void* data);

// const cl_node_t* cl_head(const cl_t* l);
// const cl_node_t* cl_tail(const cl_t* l);

// void* cl_data(const cl_node_t* const n);
// void cl_insert_between(cl_node_t* const new_node, cl_node_t* const first, cl_node_t* const second);
// int cl_is_empty(const cl_t* const l);
// cl_node_t* cl_find(int(*predicate)(const cl_node_t*), const cl_t* const l);
// const cl_node_t* cl_next(const cl_node_t* const n);
// const cl_node_t* cl_prev(const cl_node_t* const n);
// void cl_push_back(cl_node_t* n, cl_t* l);
// void cl_push_front(cl_node_t* const n, cl_t* const l);
// int cl_remove(cl_t* l, int (*predicate)(const cl_node_t* const));
// int cl_remove_no_dealloc(cl_t* l, int (*match)(const cl_node_t* const));
// void cl_rm_node(const cl_node_t* const n, cl_t* const l);
// void cl_set_only_node(cl_node_t* const n, cl_t* const l);

#endif // _CLIST_H_
