//
// Created by Taiga on 2023/3/23.
//

#ifndef CALABASH_SRC_INCLUDE_UTIL_MD5_H_
#define CALABASH_SRC_INCLUDE_UTIL_MD5_H_

#include "string"
#include "vector"

namespace calabash {

class MD5 {
 public:
  MD5() = default;
  std::string Encode(const std::string& src);

 private:
  static inline unsigned int F(unsigned int b, unsigned int c, unsigned int d);
  static inline unsigned int G(unsigned int b, unsigned int c, unsigned int d);
  static inline unsigned int H(unsigned int b, unsigned int c, unsigned int d);
  static inline unsigned int I(unsigned int b, unsigned int c, unsigned int d);
  static inline unsigned int shift(unsigned int a, unsigned int n);

  void IterateFunc(unsigned int *X);
  std::vector<unsigned int> Padding(const std::string& src);
  static std::string Format(unsigned int num);

 private:
  unsigned int temp_a_{};
  unsigned int temp_b_{};
  unsigned int temp_c_{};
  unsigned int temp_d_{};
  unsigned int str_length_{};
};


inline unsigned int MD5::F(unsigned int b, unsigned int c, unsigned int d) {
  return (b & c) | ((~b) & d);
}

inline unsigned int MD5::G(unsigned int b, unsigned int c, unsigned int d) {
  return (b & d) | (c & (~d));
}

inline unsigned int MD5::H(unsigned int b, unsigned int c, unsigned int d) {
  return b ^ c ^ d;
}

inline unsigned int MD5::I(unsigned int b, unsigned int c, unsigned int d) {
  return c ^ (b | (~d));
}

inline unsigned int MD5::shift(unsigned int a, unsigned int n) {
  return (a << n) | (a >> (32 - n));
}

} // namespace calabash

#endif //CALABASH_SRC_INCLUDE_UTIL_MD5_H_
