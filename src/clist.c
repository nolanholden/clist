#include "../include/clist/clist.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void failif(int x) { if (x) exit(x); }

struct cl_node_t {
  cl_node_t* prev;
  cl_node_t* next;
  void* data;
};

struct cl_t {
  size_t count;
  cl_node_t* head;
  cl_node_t* tail;
};

cl_t* cl_alloc_list() {
  cl_t* list = (cl_t*)malloc(sizeof(cl_t));
  failif(!list);

  list->head = NULL;
  list->tail = NULL;
  return list;
}

size_t cl_size(const cl_t* l) { return l->count; }

// cl_node_t* cl_alloc_node(void* data) {
//   cl_node_t* node = (cl_node_t*)malloc(sizeof(cl_node_t));
//   failif(!node);

//   node->next  = NULL;
//   node->prev = NULL;
//   node->data = data;
//   return node;
// }

// const cl_node_t* cl_head(const cl_t* l) { return l->head; }
// const cl_node_t* ccl_tail(const cl_t* l) { return l->tail; }

// const cl_node_t* cl_next(const cl_node_t* const n) { return n->next; }
// const cl_node_t* cl_prev(const cl_node_t* const n) { return n->prev; }
// void* cl_data(const cl_node_t* const n) { return n->data; }

// // Return whether the list is empty. Always O(1).
// int cl_is_empty(const cl_t* const l) { return l->head == NULL && l->tail == NULL; }

// // Assuming first and second are connected nodes, insert the new node, 
// // updating the pointers to reflect the change.
// void cl_insert_between(cl_node_t* const new_node, cl_node_t* const first, cl_node_t* const second) {
//   first->next = new_node;
//   new_node->prev = first;

//   second->prev = new_node;
//   new_node->next = second;
// }

// // Set the node as the head and tail of the list. This does not check if list
// // is empty. If used improperly, data will be lost, memory potentially leaked.
// void cl_set_only_node(cl_node_t* const n, cl_t* const l) {
//   l->head = n;
//   l->tail = n;
//   l->count = 1;
//   n->prev = n->next = NULL;
// }

// // Push a node to the front. If the list is empty, node will become head and
// // tail.
// void cl_push_front(cl_node_t* const n, cl_t* const l) {
//   if (is_empty(l)) return set_only_node(n, l);
  
//   l->head->prev = n;
//   n->next = l->head;
//   n->prev = NULL; // enforce no previous for head node
//   l->head = n;
//   l->count++;
// }

// // Find a node in the list which matches the predicate. If predicate returns 0,
// // no match; for return != 0, it is match.
// cl_node_t* cl_find(int(*predicate)(const cl_node_t*), const cl_t* const l) {
//   for (const cl_node_t* n = l->head; n != NULL; n = n->next)
//     if (predicate(n)) return n;
// }

// void cl_rm_node(const cl_node_t* const n, cl_t* const l) {
//   if (l->head == n){
//     if (l->tail == n) l->head = l->tail = NULL;
//     else l->head = n->next;
//   }
//   if (n->prev && n->next) { // not front or end
//       n->next->prev = n->prev;
//       n->prev->next = n->next;
//   }

//   if (n->next && !n->prev) {
//     n->next->prev = NULL;
//   }
// }

// void cl_push_back(cl_node_t* n, cl_t* l) {
//   if (is_empty(l)) return set_only_node(n, l);

//   cl_node_t* const tail_node = l->tail;

//   tail_node->next = n;
//   n->prev = tail_node;
//   l->tail = n;
// }

// // Based on a predicate which returns non-zero to indicate a node should be 
// // removed, removes the applicable nodes and returns how many were removed;
// int cl_remove_no_dealloc(cl_t* l, int (*match)(const cl_node_t* const)) {
//   int num_removed = 0;

//   for (cl_node_t* node = l->head; node != NULL; node = node->next) {
//     if (match(node)) {
//       node->next->prev = node->prev;
//       node->prev = node->next;
//       node->prev = node->next = NULL;
//       free(node->data);
//       node->data = NULL;
//       free(node);
//       node = NULL;

//     }
//   }

//   return num_removed;
// }

// int cl_remove(cl_t* l, int (*predicate)(const cl_node_t* const)) { return 0; }

#ifdef __cplusplus
}
#endif
