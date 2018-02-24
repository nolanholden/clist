#include "../include/clist/clist.h"

#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

// node
struct cl_node {
  cl_node_t* prev;
  cl_node_t* next;
  void* data;
};

// list
struct cl {
  size_t count;
  cl_node_t* head;
  cl_node_t* tail;
};

// sorted list
struct csl {
  size_t count;
  cl_node_t* head;
  cl_node_t* tail;
  int(*greater_than)(const void* first, const void* second);
};


// Common cl_t and csl_t functions:
#define CL_DECLARE_COMMON_FUNCTIONS(prefix, type) \
  size_t \
  prefix##_size(const type* l) { return l->count; } \
  const cl_node_t* \
  prefix##_peek_front(const type* l) { return l->head; } \
  const cl_node_t* \
  prefix##_peek_back(const type* l) { return l->tail; } \
  void \
  prefix##_destroy(type* const l, void(*destroy_data)(void* data)) { \
    if (l == NULL) return; \
    \
    for (cl_node_t* n = l->head; n != NULL;) { \
      if (destroy_data != NULL) destroy_data(n->data); \
      cl_node_t* tmp = n; \
      n = n->next; \
      free(tmp); \
    } \
    \
    l->count = 0; \
    free(l); \
  } \
  void \
  prefix##_foreach(type* l, void(*use_data)(void* data)) { \
    if (l == NULL || use_data == NULL) return; \
    for (const cl_node_t* n = prefix##_peek_front(l); n != NULL; n = cl_next(n)) { \
      use_data(cl_data(n)); \
    } \
  } \
  cl_node_t* \
  prefix##_find(const type* const l, int(*predicate)(const void* data)) { \
    if (l == NULL || l->count < 1 || predicate == NULL) return NULL; \
    \
    for (cl_node_t* n = l->head; n != NULL; n = n->next) \
      if (predicate(n->data)) \
        return n; \
    \
    return NULL; \
  } \
  cl_node_t* \
  prefix##_pop_back(type* const l) { \
    if (l == NULL || l->count < 1) return NULL; \
    \
    cl_node_t* const popped = l->tail; \
    \
    if (l->count-- == 1) { \
      l->head = l->tail = NULL; \
      return popped; \
    } \
    \
    l->tail = popped->prev; \
    l->tail->next = NULL; \
    \
     popped->next = popped->prev = NULL; \
    return popped; \
  } \
  void* \
  prefix##_popd_back(type* const l) { \
    cl_node_t* n = prefix##_pop_back(l); \
    void* data = cl_data(n); \
    cl_destroy_node(n, NULL); \
    return data; \
  } \
  cl_node_t* \
  prefix##_pop_front(type* const l) { \
    if (l == NULL || l->count < 1) return NULL; \
    \
    cl_node_t* const popped = l->head; \
    \
    if (l->count-- == 1) { \
      l->head = l->tail = NULL; \
      return popped; \
    } \
    \
    l->head = popped->next; \
    l->head->prev = NULL; \
    \
    popped->next = popped->prev = NULL; \
    return popped; \
  } \
  void* \
  prefix##_popd_front(type* l) { \
    cl_node_t* n = prefix##_pop_front(l); \
    void* data = cl_data(n); \
    cl_destroy_node(n, NULL); \
    return data; \
  } \
  int \
  prefix##_is_empty(const type* const l) { return l == NULL || l->count == 0; }

// end CL_DECLARE_COMMON_FUNCTIONS

CL_DECLARE_COMMON_FUNCTIONS(cl, cl_t);
CL_DECLARE_COMMON_FUNCTIONS(csl, csl_t);


// cl_node_t functions:
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
void cl_destroy_node(cl_node_t* const n, void(*destroy_data)(void* data)) {
  if (n == NULL) return;
  if (destroy_data != NULL) destroy_data(n->data);
  free(n);
}


// cl_t only functions:
cl_t* cl_alloc() {
  cl_t* list = (cl_t*)malloc(sizeof(*list));
  if (!list) return NULL;

  list->count = 0;
  list->head = NULL;
  list->tail = NULL;

  return list;
}
void cl_empl_back(cl_t* l, void* data) { cl_push_back(l, cl_alloc_node(data)); }
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


// csl_t only functions:
csl_t* csl_alloc(int (*greater_than)(const void* first, const void* second)) {
  if (!greater_than) return NULL;

  csl_t* list = (csl_t*)malloc(sizeof(*list));
  if (!list) return NULL;

  list->count = 0;
  list->head = NULL;
  list->tail = NULL;
  list->greater_than = greater_than;

  return list;
}
void csl_insert(csl_t* l, cl_node_t* new_node) {
  if (!new_node) return;
  
  if (!l->head || l->greater_than(l->head->data, new_node->data)) {
      new_node->next = l->head;
      l->head = new_node;
  } else {
    // find the node before the insertion point
    cl_node_t* itr = l->head;
    while (itr->next
        && !l->greater_than(itr->next->data, new_node->data)) {
      itr = itr->next;
    }
    new_node->next = itr->next;
    itr->next = new_node;
  }

  ++(l->count);
}
void csl_insertd(csl_t* l, void* data) { csl_insert(l, cl_alloc_node(data)); }

#ifdef __cplusplus
}
#endif
