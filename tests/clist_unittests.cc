#include <gtest/gtest.h>

#include "../include/clist/clist.h"

#include <stdlib.h>
#include <string.h>
#include <time.h> // for rand() seeding

void simple_free(void* data) {
  free(data);
}

TEST(clist, initialize_list) {
  ASSERT_EQ(0,0);
  cl_t* l = cl_alloc_list();
  ASSERT_EQ(0, cl_size(l));
  ASSERT_EQ(NULL, cl_peak_front(l));
  ASSERT_EQ(NULL, cl_peak_back(l));
  cl_destroy(l, simple_free);
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

  cl_destroy_node(n, simple_free);
}

TEST(clist, push_pop_one) {
  {
    cl_node_t* n = cl_alloc_node(strdup("hello, world"));
    cl_t* l = cl_alloc_list();
    cl_push_front(l, n);
    ASSERT_EQ(1, cl_size(l));
    ASSERT_EQ(n, cl_peak_front(l));
    ASSERT_EQ(n, cl_peak_back(l));
    cl_destroy(l, simple_free);
  }
  {
    cl_node_t* n = cl_alloc_node(strdup("hello, world"));
    cl_t* l = cl_alloc_list();
    cl_push_back(l, n);
    ASSERT_EQ(1, cl_size(l));
    ASSERT_EQ(n, cl_peak_front(l));
    ASSERT_EQ(n, cl_peak_back(l));
    cl_destroy(l, simple_free);
  }
}

TEST(clist, pushing) {
  cl_node_t* n1 = cl_alloc_node(strdup("hello, world1"));
  cl_node_t* n2 = cl_alloc_node(strdup("hello, world2"));
  cl_node_t* n3 = cl_alloc_node(strdup("hello, world3"));
  cl_node_t* n4 = cl_alloc_node(strdup("hello, world4"));
  cl_node_t* n5 = cl_alloc_node(strdup("hello, world5"));
  cl_t* l = cl_alloc_list();

  cl_push_front(l, n2);
  cl_push_back (l, n3);
  ASSERT_EQ(2, cl_size(l));
  ASSERT_EQ(n2, cl_peak_front(l));
  ASSERT_EQ(n3, cl_peak_back(l));
  
  cl_push_front(l, n1);
  ASSERT_EQ(3,  cl_size(l));
  ASSERT_EQ(n1, cl_peak_front(l));
  ASSERT_EQ(n3, cl_peak_back(l));
  ASSERT_EQ(NULL, cl_prev(n1));
  ASSERT_EQ(n2,   cl_next(n1));
  ASSERT_EQ(n1,   cl_prev(n2));
  ASSERT_EQ(n3,   cl_next(n2));
  ASSERT_EQ(n2,   cl_prev(n3));
  ASSERT_EQ(NULL, cl_next(n3));

  cl_push_back(l, n4);
  ASSERT_EQ(4, cl_size(l));
  ASSERT_EQ(n1, cl_peak_front(l));
  ASSERT_EQ(n4, cl_peak_back(l));

  cl_push_back(l, n5);
  ASSERT_EQ(5, cl_size(l));
  ASSERT_EQ(n1, cl_peak_front(l));
  ASSERT_EQ(n5, cl_peak_back(l));
  
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

  cl_destroy(l, simple_free);
}

TEST(clist, popping1) {
  cl_node_t* n1 = cl_alloc_node(strdup("hello, world1"));
  cl_node_t* n2 = cl_alloc_node(strdup("hello, world2"));
  cl_node_t* n3 = cl_alloc_node(strdup("hello, world3"));
  cl_node_t* n4 = cl_alloc_node(strdup("hello, world4"));
  cl_node_t* n5 = cl_alloc_node(strdup("hello, world5"));
  
  cl_t* l = cl_alloc_list();
  cl_push_back(l, n1);
  cl_push_back(l, n2);
  cl_push_back(l, n3);
  cl_push_back(l, n4);
  cl_push_back(l, n5);

  ASSERT_EQ(5, cl_size(l));
  ASSERT_EQ(n1, cl_pop_front(l));
  ASSERT_EQ(NULL, cl_prev(cl_peak_front(l)));
  ASSERT_EQ(4, cl_size(l));
  ASSERT_EQ(n2, cl_pop_front(l));
  ASSERT_EQ(3, cl_size(l));

  ASSERT_EQ(n3, cl_pop_front(l));
  ASSERT_EQ(n4, cl_pop_front(l));
  ASSERT_EQ(n5, cl_pop_front(l));
  ASSERT_EQ(0, cl_size(l));
  ASSERT_EQ(NULL, cl_pop_front(l));
  ASSERT_EQ(NULL, cl_pop_back(l));
  ASSERT_EQ(0, cl_size(l));
  
  cl_destroy_node(n1, simple_free);
  cl_destroy_node(n2, simple_free);
  cl_destroy_node(n3, simple_free);
  cl_destroy_node(n4, simple_free);
  cl_destroy_node(n5, simple_free);

  cl_destroy(l, simple_free);
}

TEST(clist, empl_back) {
  cl_t* l = cl_alloc_list();
  cl_empl_back(l, (void*)1);
  cl_empl_back(l, (void*)2);
  ASSERT_EQ((void*)1, cl_data(cl_peak_front(l)));
  ASSERT_EQ((void*)2, cl_data(cl_peak_back(l)));

  cl_destroy(l, NULL);
}

TEST(clist, empl_front) {
  cl_t* l = cl_alloc_list();
  cl_empl_front(l, (void*)2);
  cl_empl_front(l, (void*)1);
  ASSERT_EQ((void*)1, cl_data(cl_peak_front(l)));
  ASSERT_EQ((void*)2, cl_data(cl_peak_back(l)));

  cl_destroy(l, NULL);
}

TEST(clist, popping2) {
  cl_node_t* n1 = cl_alloc_node(strdup("hello, world1"));
  cl_node_t* n2 = cl_alloc_node(strdup("hello, world2"));
  cl_node_t* n3 = cl_alloc_node(strdup("hello, world3"));
  cl_node_t* n4 = cl_alloc_node(strdup("hello, world4"));
  cl_node_t* n5 = cl_alloc_node(strdup("hello, world5"));
  
  cl_t* l = cl_alloc_list();
  cl_push_back(l, n1);
  cl_push_back(l, n2);
  cl_push_back(l, n3);
  cl_push_back(l, n4);
  cl_push_back(l, n5);

  ASSERT_EQ(n1, cl_pop_front(l));
  ASSERT_EQ(n2, cl_pop_front(l));
  ASSERT_EQ(n5, cl_pop_back(l));
  ASSERT_EQ(NULL, cl_next(cl_peak_back(l)));
  ASSERT_EQ(2, cl_size(l));
  ASSERT_EQ(n3, cl_pop_front(l));
  ASSERT_EQ(n4, cl_pop_back(l));
  ASSERT_EQ(NULL, cl_pop_back(l));
  ASSERT_EQ(0, cl_size(l));
  ASSERT_EQ(NULL, cl_peak_front(l));
  ASSERT_EQ(NULL, cl_peak_back(l));
  
  cl_destroy_node(n1, simple_free);
  cl_destroy_node(n2, simple_free);
  cl_destroy_node(n3, simple_free);
  cl_destroy_node(n4, simple_free);
  cl_destroy_node(n5, simple_free);
  
  cl_destroy(l, simple_free);
}

int sort_int_nodes(const void* first, const void* second) {
  int a = *(int*)first;
  int b = *(int*)second;
  return a > b ? 1 : 0;
}

TEST(clist, validate_test_comparater) {
  int a = 1;
  ASSERT_EQ(0, sort_int_nodes(&a, &a));

  int b = 2;
  ASSERT_EQ(0, sort_int_nodes(&a, &b));
  ASSERT_EQ(1, sort_int_nodes(&b, &a));
}

TEST(clist, sort_empty) {
  cl_t* l = NULL;
  cl_sort(l, NULL);
  ASSERT_EQ(NULL, (l));
  cl_destroy(l, simple_free);

  l = cl_alloc_list();

  cl_sort(l, NULL);
  ASSERT_EQ(NULL, cl_peak_front(l));
  ASSERT_EQ(NULL, cl_peak_back(l));
  cl_destroy(l, simple_free);
}

TEST(clist, sort_one) {
  cl_t* l = cl_alloc_list();
  int* num = (int*)malloc(sizeof(*num));
  *num = 127;
  cl_push_back(l, cl_alloc_node(num));
  cl_sort(l, NULL);
  cl_sort(l, sort_int_nodes);

  cl_destroy(l, simple_free);
}

TEST(clist, sort_many) {
  for (int i = 0; i < 1000; ++i) {
    srand((unsigned int)time(NULL));
    const size_t num_nodes = 100;
    cl_t* l = cl_alloc_list();

    for (size_t j = 0; j < num_nodes; ++j) {
      int* num = (int*)malloc(sizeof(*num));
      *num = rand();
      cl_push_front(l, cl_alloc_node(num));
    }

    cl_sort(l, sort_int_nodes);

    for (const cl_node_t* n = cl_peak_front(l); n != cl_peak_back(l); n = cl_next(n)) {
      int a = *(int*)cl_data(n);
      int b = *(int*)cl_data(cl_next(n));
      ASSERT_LE(a, b);
    }

    cl_destroy(l, simple_free);
  }
}

int is_five(const void* data) { return *(int*)data == 5 ? 1 : 0; }

TEST(clist, find_zero) {
  cl_t* l = NULL;
  ASSERT_EQ(NULL, cl_find(l, NULL));

  l = cl_alloc_list();
  ASSERT_EQ(NULL, cl_find(l, NULL));
  cl_destroy(l, simple_free);
}

TEST(clist, find_one) {
  cl_t* l = cl_alloc_list();

  int* num = (int*)malloc(sizeof(*num));
  *num = 4;
  cl_push_back(l, cl_alloc_node(num));
  ASSERT_EQ(NULL, cl_find(l, is_five));

  *num = 5;
  ASSERT_EQ(num, cl_data(cl_find(l, is_five)));

  cl_destroy(l, simple_free);
}

TEST(clist, find_many) {
  for (int i = 0; i < 1000; ++i) {
    srand((unsigned int)time(NULL));
    const size_t num_nodes = 100;
    cl_t* l = cl_alloc_list();
    
    for (size_t j = 0; j < num_nodes; ++j) {
      int* num = (int*)malloc(sizeof(*num));
      *num = rand();
      if (*num == 5) *num = 6;
      cl_push_front(l, cl_alloc_node(num));
    }

    ASSERT_EQ(NULL, cl_find(l, is_five));

    cl_destroy(l, simple_free);
  }

  for (int i = 0; i < 1000; ++i) {
    srand((unsigned int)time(NULL));
    const size_t num_nodes = 100;
    cl_t* l = cl_alloc_list();
    
    const size_t select = rand() % num_nodes;
    const int* selected = NULL;
    for (size_t j = 0; j < num_nodes; ++j) {
      int* num = (int*)malloc(sizeof(*num));
      *num = 6;
      if (j == select) {
        *num = 5;
        selected = num;
      }
      cl_push_front(l, cl_alloc_node(num));
    }

    cl_node_t* found_node = cl_find(l, is_five);
    ASSERT_EQ(selected, (int*)cl_data(found_node));

    cl_destroy(l, simple_free);
  }
}

struct TestStruct {
  int* array;
  int data;
};

void free_test_struct(void* ts) {
  free(((struct TestStruct*)ts)->array);
  free(ts);
}

// Our test here is that valgrind claims unfreed blocks if the list fails to 
// use our user-defined deallocator.
TEST(clist, complex_destroy) {
  cl_t* l = cl_alloc_list();
  struct TestStruct* s = (struct TestStruct*)malloc(sizeof(*s));
  const size_t num_elements = 10000;
  s->array = (int*)malloc(sizeof(s->array[0]) * num_elements);
  cl_empl_back(l, s);

  cl_destroy(l, free_test_struct);
}
TEST(clist, complex_destroy2) {
  cl_t* l = cl_alloc_list();
  struct TestStruct* s = (struct TestStruct*)malloc(sizeof(*s));
  const size_t num_elements = 10000;
  s->array = (int*)malloc(sizeof(s->array[0]) * num_elements);
  cl_empl_back(l, s);
  cl_destroy_node(cl_pop_back(l), free_test_struct);

  cl_destroy(l, NULL);
}

int num_calls = 0;
void inc_by_one(void* data) {
  ++num_calls;
  ++(*(int*)data);
}

void assert_incremented(void* data) {
  static int expected_value = 1;
  ASSERT_EQ(expected_value, *(int*)data);
  ++expected_value;
}

TEST(clist, foreach) {
  cl_t* l = cl_alloc_list();
  const int size = 100;
  int nums[size];
  for (size_t i = 0; i < size; ++i) {
    nums[i] = (int)i;
    cl_empl_back(l, &nums[i]);
  }

  cl_foreach(l, inc_by_one);
  ASSERT_EQ(size, num_calls);

  int expected_value = 1;
  for (const cl_node_t* n = cl_peak_front(l); n != NULL; n = cl_next(n)) {
    ASSERT_EQ(expected_value, *(int*)cl_data(n));
    ++expected_value;
  }

  cl_foreach(l, assert_incremented);

  cl_destroy(l, NULL);
}

// // iterator
// TEST(clist, iterator) {
//   cl_t* l = cl_alloc_list();
//   const size_t size = 100;
//   int nums[size];
//   for (size_t i = 0; i < size; ++i) {
//     nums[i] = i;
//     cl_empl_back(l, &nums[i]);
//   }



//   for (cl_itr* i = cl_itr(l); i != NULL; i = cl_itr_next(i))

// }