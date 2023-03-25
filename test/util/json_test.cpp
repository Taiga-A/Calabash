//
// Created by Taiga on 2023/3/23.
//

#include "nlohmann/json.hpp"

#include "gtest/gtest.h"

using namespace nlohmann;
using namespace std;

namespace calabash {

TEST(json_test, base_test) {
  json j = json::parse(R"({"num":123})");
  ASSERT_EQ(j.type(), json::value_t::object);
  ASSERT_EQ(j["num"], 123);

  vector<int> test_arr = {113, 456, 789};
  j["data"] = test_arr;
  ASSERT_EQ(j["data"].type(), json::value_t::array);
  ASSERT_EQ(j["data"].size(), test_arr.size());
  for (int i = 0; i < test_arr.size(); i++) {
    ASSERT_EQ(j["data"][i], test_arr[i]);
  }

  try {
    json::parse(R"({123---})").type();
    ASSERT_EQ("json parse success", "json parse err");
  } catch (const json::exception& e) {
    ASSERT_EQ("json parse err", "json parse err");
  }

}

} // namespace calabash
