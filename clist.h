#ifndef _CLIST_H_
#define _CLIST_H_

#include <stdint.h>

typedef struct Clist cl_t;
typedef struct Node node_t;

cl_t* cl_alloc_list();
node_t* cl_alloc_node(void* data);

const node_t* cl_head(const cl_t* l);
const node_t* cl_tail(const cl_t* l);
int cl_size(const cl_t* l);

void* cl_data(const node_t* const n);
void cl_insert_between(node_t* const new_node, node_t* const first, node_t* const second);
int cl_is_empty(const cl_t* const l);
node_t* cl_find(int(*predicate)(const node_t*), const cl_t* const l);
const node_t* cl_next(const node_t* const n);
const node_t* cl_prev(const node_t* const n);
void cl_push_back(node_t* n, cl_t* l);
void cl_push_front(node_t* const n, cl_t* const l);
int cl_remove(cl_t* l, int (*predicate)(const node_t* const));
int cl_remove_no_dealloc(cl_t* l, int (*match)(const node_t* const));
void cl_rm_node(const node_t* const n, cl_t* const l);
void cl_set_only_node(node_t* const n, cl_t* const l);

#endif // _CLIST_H_
