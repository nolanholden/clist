#include "../include/clist/clist.h"

#include <stddef.h>
#include <stdlib.h>

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

// end non-exported helpers

cl_t* cl_alloc_list() {
  cl_t* list = (cl_t*)malloc(sizeof(cl_t));
  if (!list) return NULL;

  list->count = 0;
  list->head = NULL;
  list->tail = NULL;
  return list;
}

size_t cl_size(const cl_t* l) { return l->count; }
const cl_node_t* cl_peak_front(const cl_t* l) { return l->head; }
const cl_node_t* cl_peak_back(const cl_t* l) { return l->tail; }

cl_node_t* cl_alloc_node(void* data) {
  cl_node_t* node = (cl_node_t*)malloc(sizeof(cl_node_t));
  if (!node) return NULL;

  node->next  = NULL;
  node->prev = NULL;
  node->data = data;
  return node;
}

const cl_node_t* cl_next(const cl_node_t* const n) { return n->next; }
const cl_node_t* cl_prev(const cl_node_t* const n) { return n->prev; }
void* cl_data(const cl_node_t* const n) { return n->data; }
void cl_destroy(cl_t* const l, void(*destroy_data)(void* data)) {
  if (l == NULL) return;

  for (cl_node_t* n = l->head; n != NULL;) {
    if (destroy_data != NULL) destroy_data(n->data);
    cl_node_t* tmp = n;
    n = n->next;
    free(tmp);
  }

  l->count = 0;
  free(l);
}

void cl_destroy_node(cl_node_t* const n, void(*destroy_data)(void* data)) {
  if (n == NULL) return;
  if (destroy_data != NULL) destroy_data(n->data);
  free(n);
}

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

void cl_foreach(cl_t* l, void(*use_data)(void* data)) {
  if (l == NULL || use_data == NULL) return;
  for (const cl_node_t* n = cl_peak_front(l); n != NULL; n = cl_next(n)) {
    use_data(cl_data(n));
  }
}

void cl_empl_front(cl_t* l, void* data) { cl_push_front(l, cl_alloc_node(data)); }

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

void cl_empl_back(cl_t* l, void* data) { cl_push_back(l, cl_alloc_node(data)); }

cl_node_t* cl_pop_back(cl_t* const l) {
  if (l == NULL || l->count < 1) return NULL;

  cl_node_t* const popped = l->tail;

  if (l->count-- == 1) {
    l->head = l->tail = NULL;
    return popped;
  }

  l->tail = popped->prev;
  l->tail->next = NULL;

  popped->next = popped->prev = NULL;
  return popped;
}

void* cl_popd_back(cl_t* const l) {
  cl_node_t* n = cl_pop_back(l);
  void* data = cl_data(n);
  cl_destroy_node(n, NULL);
  return data;
}

cl_node_t* cl_pop_front(cl_t* const l) {
  if (l == NULL || l->count < 1) return NULL;
  
  cl_node_t* const popped = l->head;
  
  if (l->count-- == 1) {
    l->head = l->tail = NULL;
    return popped;
  }

  l->head = popped->next;
  l->head->prev = NULL;

  popped->next = popped->prev = NULL;
  return popped;
}

void* cl_popd_front(cl_t* l) {
  cl_node_t* n = cl_pop_front(l);
  void* data = cl_data(n);
  cl_destroy_node(n, NULL);
  return data;
}

int cl_is_empty(const cl_t* const l) { return l == NULL || l->count == 0; }

void cl_sort(cl_t* l, int(*greater_than)(const void* first, const void* second)) {
  if (l == NULL || l->count <= 1 || greater_than == NULL) return;

  for (cl_node_t* n = l->head->next; n != NULL; n = n->next) {
    for (cl_node_t* o = n; o != l->head && greater_than(o->prev->data, o->data); o = o->prev) {
      void* tmp = o->data;
      o->data = o->prev->data;
      o->prev->data = tmp;
    }
  }
}

cl_node_t* cl_find(const cl_t* const l, int(*predicate)(const void* data)) {
  if (l == NULL || l->count < 1 || predicate == NULL) return NULL;

  for (cl_node_t* n = l->head; n != NULL; n = n->next)
    if (predicate(n->data))
      return n;

  return NULL;
}

#ifdef __cplusplus
}
#endif
