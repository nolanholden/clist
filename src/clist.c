#include "../include/clist/clist.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

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

// non-exported helpers:

void failif(int x) { if (x) exit(x); }

enum {
  LMARGIN = 0,
  LEFT    = 1,
  RIGHT   = 2,
  RMARGIN = 3
};

// end non-exported helpers

cl_t* cl_alloc_list() {
  cl_t* list = (cl_t*)malloc(sizeof(cl_t));
  failif(!list);

  list->count = 0;
  list->head = NULL;
  list->tail = NULL;
  return list;
}

size_t cl_size(const cl_t* l) { return l->count; }
cl_node_t* cl_head(const cl_t* l) { return l->head; }
cl_node_t* cl_tail(const cl_t* l) { return l->tail; }

cl_node_t* cl_alloc_node(void* data) {
  cl_node_t* node = (cl_node_t*)malloc(sizeof(cl_node_t));
  failif(!node);

  node->next  = NULL;
  node->prev = NULL;
  node->data = data;
  return node;
}

const cl_node_t* cl_next(const cl_node_t* const n) { return n->next; }
const cl_node_t* cl_prev(const cl_node_t* const n) { return n->prev; }
void* cl_data(const cl_node_t* const n) { return n->data; }
void cl_destroy(cl_t* const l) {
  if (l == NULL) return;

  for (cl_node_t* n = l->head; n != NULL;) {
    free(n->data);
    cl_node_t* tmp = n;
    n = n->next;
    free(tmp);
  }

  l->count = 0;
  free(l);
}

void cl_destroy_node(cl_node_t* const n) {
  if (n == NULL) return;
  free(n->data);
  free(n);
}


// Push a node to the front. If the list is empty, node will become head and
// tail.
void cl_push_front(cl_t* const l, cl_node_t* const n) {
  if (l == NULL || n == NULL) return;
  if (cl_is_empty(l)) {
    n->next = n->prev = NULL;
    l->head = l->tail = n;
    l->count = 1;
    return;
  }
  
  n->prev = NULL;
  n->next = l->head;
  l->head->prev = n;
  l->head = n;
  l->count++;
}

void cl_push_back(cl_t* const l, cl_node_t* const n) {
  if (l == NULL || n == NULL) return;
  if (cl_is_empty(l)) {
    n->next = n->prev = NULL;
    l->head = l->tail = n;
    l->count = 1;
    return;
  }

  n->next = NULL;
  n->prev = l->tail;
  l->tail->next = n;
  l->tail = n;
  l->count++;
}

cl_node_t* cl_pop_back(cl_t* const l) {
  if (l == NULL || l->tail == NULL) return NULL;

  cl_node_t* const back = l->tail;
  cl_node_t* const left = back->prev;

  l->tail = left;
  left->next = NULL;

  back->next = back->prev = NULL;
  l->count--;
  return back;
}

cl_node_t* cl_pop_front(cl_t* const l) {
  if (l == NULL || l->tail == NULL) return NULL;

  cl_node_t* const front = l->tail;
  cl_node_t* const right = right->next;

  l->head = right;
  right->prev = NULL;
  
  front->next = front->prev = NULL;
  l->count--;
  return front;
}

// Return whether the list is empty. Always O(1).
int cl_is_empty(const cl_t* const l) { return l == NULL || l->count == 0; }

void cl_sort(cl_t* l, int(*greater_than)(void* first, void* second)) {
  if (l == NULL || l->count <= 1 || greater_than == NULL) return;

  for (cl_node_t* n = l->head->next; n != NULL; n = n->next) {
    for (cl_node_t* o = n; o != l->head && greater_than(o->prev->data, o->data); o = o->prev) {
      void* tmp = o->data;
      o->data = o->prev->data;
      o->prev->data = tmp;
    }
  }
}

// Find a node in the list which matches the predicate. If predicate returns 0,
// no match; for return != 0, it is match.
cl_node_t* cl_find(int(*predicate)(const cl_node_t*), const cl_t* const l) {
  for (cl_node_t* n = l->head; n != NULL; n = n->next)
    if (predicate(n))
      return n;

  return NULL;
}

#ifdef __cplusplus
}
#endif
