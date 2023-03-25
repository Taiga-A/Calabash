//
// Created by Taiga on 2023/3/24.
//

#ifndef CALABASH_SRC_INCLUDE_UTIL_BASE64_H_
#define CALABASH_SRC_INCLUDE_UTIL_BASE64_H_

#include "string"

namespace calabash {

class Base64 {
 public:
  [[maybe_unused]] static std::string Encode(const std::string &input);
  [[maybe_unused]] static std::string Decode(const std::string &input);
};

} // namespace calabash



#endif //CALABASH_SRC_INCLUDE_UTIL_BASE64_H_
