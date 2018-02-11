#include <gtest/gtest.h>

#include "../include/clist/clist.h"

#include <stdlib.h>
#include <string.h>

TEST(cl, initialization) {
  ASSERT_EQ(0,0);
  cl_t* l = cl_alloc_list();
  ASSERT_EQ(0, cl_size(l));
  // ASSERT_EQ(NULL, cl_head(l));
  // ASSERT_EQ(NULL, cl_tail(l));
}

// TEST(cl_node, initialization) {
//   char fifteen_chars[] = "123456789abcde";
//   size_t count = 1 + strlen(fifteen_chars);
//   char* buffer = (char*)malloc(sizeof(char) * count);
//   strcpy(buffer, fifteen_chars);

//   cl_node_t* n = cl_alloc_node(buffer);
//   ASSERT_EQ(NULL, cl_next(n));
//   ASSERT_EQ(NULL, cl_prev(n));
  
//   for (int i = 0; i < 15; ++i)
//     ASSERT_EQ(fifteen_chars[i], ((char*)cl_data(n))[i]);
// }
