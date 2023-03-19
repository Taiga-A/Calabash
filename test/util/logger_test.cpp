//
// Created by Taiga on 2023/3/19.
//

#include "util/logger.h"

#include <gtest/gtest.h>

namespace calabash {

TEST(logger_test, output_test) {
  std::cout << "hello googletest" << std::endl;
}

} // namespace calabash