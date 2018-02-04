#include <stdlib.h>

typedef struct Node {
  struct Node* prev;
  struct Node* next;
  void* data;
} node_t;

node_t* const next(const node_t* const n) { return n->next; }
node_t* const prev(const node_t* const n) { return n->prev; }
void* data(const node_t* const n) { return n->data; }

typedef struct List {
  int count;
  node_t* head;
  node_t* tail;
} list_t;

void failif(int x) { if (x) exit(x); }

node_t* alloc_node(void* data) {
  node_t* node = malloc(sizeof(node_t));
  failif(!node);

  node->next  = NULL;
  node->prev = NULL;
  node->data = data;
  return node;
}

list_t* alloc_list() {
  list_t* list = malloc(sizeof(list_t));
  failif(!list);

  list->head = NULL;
  list->tail = NULL;
  return list;
}


// Return whether the list is empty. Always O(1).
int is_empty(const list_t* const l) { return l->head == NULL && l->tail == NULL; }

// Assuming first and second are connected nodes, insert the new node, 
// updating the pointers to reflect the change.
void insert_between(node_t* const new_node, node_t* const first, node_t* const second) {
  assert(first->next == second);
  assert(second->prev == first);
  
  first->next = new_node;
  new_node->prev = first;

  second->prev = new_node;
  new_node->next = second;
}

// Set the node as the head and tail of the list. This does not check if list
// is empty. If used improperly, data will be lost, memory potentially leaked.
void set_only_node(node_t* const n, list_t* const l) {
  l->head = n;
  l->tail = n;
  l->count = 1;
  n->prev = n->next = NULL;
}

// Push a node to the front. If the list is empty, node will become head and
// tail.
void push_front(node_t* const n, list_t* const l) {
  if (is_empty(l)) return set_only_node(n, l);
  
  l->head->prev = n;
  n->next = l->head;
  n->prev = NULL; // enforce no previous for head node
  l->head = n;
  l->count++;
}

// Find a node in the list which matches the predicate. If predicate returns 0,
// no match; for return != 0, it is match.
node_t* find(int(*predicate)(const node_t*), const list_t* const l) {
  for (const node_t* n = l->head; n != NULL; n = n->next)
    if (predicate(n)) return n;
}

void rm_node(const node_t* const n, list_t* const l) {
  if (l->head == n){
    if (l->tail == n) l->head = l->tail = NULL;
    else l->head = n->next;
  }
  if (n->prev && n->next) { // not front or end
      n->next->prev = n->prev;
      n->prev->next = n->next;
  }

  if (n->next && !n->prev) {
    n->next->prev = NULL;
  }



void push_back(node_t* n, list_t* l) {
  if (is_empty(l)) return set_only_node(n, l);

  node_t* const tail_node = l->tail;

  tail_node->next = n;
  n->prev = tail_node;
  l->tail = n;
}

// Based on a predicate which returns non-zero to indicate a node should be 
// removed, removes the applicable nodes and returns how many were removed;
int remove_no_dealloc(list_t* l, int (*match)(const node_t* const)) {
  int num_removed = 0;

  for (node_t* node = l->head; node != NULL; node = node->next) {
    if (match(node)) {
      node->next->prev = node->prev;
      node->prev = node->next;
      node->prev = node->next = NULL;
      free(node->data);
      node->data = NULL;
      free(node);
      node = NULL;

    }
  }

  return num_removed;
}

int remove(list_t* l, int (*predicate)(const node_t* const)) {
