//
// Created by Taiga on 2023/3/23.
//

#include "util/md5.h"

#include "gtest/gtest.h"

using namespace calabash;
using namespace std;

TEST(md5_test, base_test) {
  MD5 parser;
  string res;

  res = parser.Encode("123456");
  ASSERT_EQ(res, "E10ADC3949BA59ABBE56E057F20F883E");

  res = parser.Encode("789123");
  ASSERT_EQ(res, "0D06FD8CB26EB57F1A690F493663CC55");

  res = parser.Encode("qwertyuiopasdfghjklzxcvbnm123456789qwertyuiopasdfghjklzxcvbnm123456789");
  ASSERT_EQ(res, "255B794D440F080FCE958228D6DFB614");
}