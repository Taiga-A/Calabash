//
// Created by Taiga on 2023/3/23.
//

#include "gtest/gtest.h"
#include "vector"
#include "thread"
#include "thread/thread_pool.h"

using namespace calabash;
using namespace std;

TEST(thread_pool_test, base_test) {
  auto tp = new ThreadPool(10);
  const int test_arr_size = 100;
  vector<int> test_arr(test_arr_size);
  for (int i = 0; i < test_arr_size; i++) {
    tp->Enqueue([&](){
      test_arr[i] = i;
      std::this_thread::sleep_for(10ms);
    });
  }
  delete tp;
  for (int i = 0; i < test_arr_size; i++) {
    ASSERT_EQ(test_arr[i], i);
  }
}