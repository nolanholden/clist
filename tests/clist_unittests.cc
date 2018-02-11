#include <gtest/gtest.h>

#include "../include/clist/clist.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>

TEST(clist, initialize_list) {
  ASSERT_EQ(0,0);
  cl_t* l = cl_alloc_list();
  ASSERT_EQ(0, cl_size(l));
  ASSERT_EQ(NULL, cl_head(l));
  ASSERT_EQ(NULL, cl_tail(l));
  cl_destroy(l);
}

TEST(clist, initialize_node) {
  char fifteen_chars[] = "123456789abcde";
  size_t num_chars = sizeof(fifteen_chars) / sizeof(fifteen_chars[0]);
  char* buffer = (char*)malloc(sizeof(*buffer) * num_chars);
  strcpy(buffer, fifteen_chars);

  cl_node_t* n = cl_alloc_node(buffer);
  ASSERT_EQ(NULL, cl_next(n));
  ASSERT_EQ(NULL, cl_prev(n));
  
  for (int i = 0; i < 15; ++i)
    ASSERT_EQ(fifteen_chars[i], ((char*)cl_data(n))[i]);

  cl_destroy_node(n);
}

TEST(clist, push_pop_one) {
  {
    cl_node_t* n = cl_alloc_node(strdup("hello, world"));
    cl_t* l = cl_alloc_list();
    cl_push_front(l, n);
    ASSERT_EQ(1, cl_size(l));
    ASSERT_EQ(n, cl_head(l));
    ASSERT_EQ(n, cl_tail(l));
    cl_destroy(l);
  }
  {
    cl_node_t* n = cl_alloc_node(strdup("hello, world"));
    cl_t* l = cl_alloc_list();
    cl_push_back(l, n);
    ASSERT_EQ(1, cl_size(l));
    ASSERT_EQ(n, cl_head(l));
    ASSERT_EQ(n, cl_tail(l));
    cl_destroy(l);
  }
}

TEST(clist, push_pop) {
  cl_node_t* n1 = cl_alloc_node(strdup("hello, world1"));
  cl_node_t* n2 = cl_alloc_node(strdup("hello, world2"));
  cl_node_t* n3 = cl_alloc_node(strdup("hello, world3"));
  cl_node_t* n4 = cl_alloc_node(strdup("hello, world4"));
  cl_node_t* n5 = cl_alloc_node(strdup("hello, world5"));
  cl_t* l = cl_alloc_list();

  cl_push_front(l, n2);
  cl_push_back (l, n3);
  ASSERT_EQ(2, cl_size(l));
  ASSERT_EQ(n2, cl_head(l));
  ASSERT_EQ(n3, cl_tail(l));
  
  cl_push_front(l, n1);
  ASSERT_EQ(3,  cl_size(l));
  ASSERT_EQ(n1, cl_head(l));
  ASSERT_EQ(n3, cl_tail(l));
  ASSERT_EQ(NULL, cl_prev(n1));
  ASSERT_EQ(n2,   cl_next(n1));
  ASSERT_EQ(n1,   cl_prev(n2));
  ASSERT_EQ(n3,   cl_next(n2));
  ASSERT_EQ(n2,   cl_prev(n3));
  ASSERT_EQ(NULL, cl_next(n3));

  cl_push_back(l, n4);
  ASSERT_EQ(4, cl_size(l));
  ASSERT_EQ(n1, cl_head(l));
  ASSERT_EQ(n4, cl_tail(l));

  cl_push_back(l, n5);
  ASSERT_EQ(5, cl_size(l));
  ASSERT_EQ(n1, cl_head(l));
  ASSERT_EQ(n5, cl_tail(l));
  
  cl_node_t* nodes[] = { n1, n2, n3, n4, n5 };
  const size_t num_worlds = sizeof(nodes) / sizeof(nodes[0]);

  for (size_t i = 0; i < num_worlds - 1; ++i) {
    ASSERT_EQ(nodes[i+1], cl_next(nodes[i]));
  }
  for (size_t i = num_worlds; i < 1; ++i) {
    ASSERT_EQ(nodes[i-1], cl_prev(nodes[i]));
  }

  char test_str[] = "hello, worldx";
  const size_t world_idx = 12;
  for (size_t i = 0; i < num_worlds; ++i) {
    test_str[world_idx] = (char)('1' + i);
    ASSERT_STREQ(test_str, (char*)cl_data(nodes[i]));
  }

  cl_destroy(l);
}

int sort_int_nodes(cl_node_t* first, cl_node_t* second) {
  int a = *(int*)cl_data(first);
  int b = *(int*)cl_data(second);
  return a == b ? 0
    : a > b ? 1 : -1;
}

TEST(clist, sort) {
  srand(time(NULL));
  const size_t num_nodes = 100;
  cl_t* l = cl_alloc_list();

  for (size_t i = 0; i < num_nodes; ++i) {
    int* num = (int*)malloc(sizeof(*num));
    *num = rand();
    cl_push_front(l, cl_alloc_node(num));
  }

  cl_sort(l, sort_int_nodes);

  for (cl_node_t* n = cl_head(l); n != cl_tail(l); n = cl_next(n)) {
    
  }
}