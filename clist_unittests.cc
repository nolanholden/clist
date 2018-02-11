#include "clist.h"

#include <gtest/gtest.h>

#include <stdlib.h>

TEST(cl, initialization) {
  cl_t* l = cl_alloc_list();
  ASSERT_EQ(0, cl_size(l));
  ASSERT_EQ(NULL, cl_head(l));
  ASSERT_EQ(NULL, cl_tail(l));
}

TEST(cl_node, initialization) {
  char fifteen_chars[] = "123456789abcde";
  int count = 15;
  char* buffer = (char*)malloc(sizeof(char) * count);
  strcpy(buffer, fifteen_chars);

  node_t* node = cl_alloc_node(buffer);
  ASSERT_EQ(NULL, cl_next(node));
  ASSERT_EQ(NULL, cl_prev(node));
  
  for (int i = 0; i < 15; ++i)
    ASSERT_EQ(fifteen_chars[i], ((char*)cl_data(node))[i]);
}
