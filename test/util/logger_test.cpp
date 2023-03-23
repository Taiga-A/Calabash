//
// Created by Taiga on 2023/3/19.
//

#include "util/logger.h"

#include <gtest/gtest.h>

namespace calabash {

TEST(logger_test, output_test) {
  ASSERT_EQ(123, 123);
  std::cout << "hello googletest" << std::endl;


}

TEST(logger_test, input_test) {
//  ASSERT_EQ(123, 456);
  std::cout << "hello googletest" << std::endl;


}

} // namespace calabash