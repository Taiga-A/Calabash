//
// Created by Taiga on 2023/3/23.
//

#include <vector>
#include <string>

#include "util/md5.h"

using namespace std;
using namespace calabash;

constexpr unsigned int A = 0x67452301;
constexpr unsigned int B = 0xefcdab89;
constexpr unsigned int C = 0x98badcfe;
constexpr unsigned int D = 0x10325476;

const char str16[] = "0123456789ABCDEF";

const unsigned int T[] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};

const unsigned int s[] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
                          5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
                          4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
                          6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};

std::string MD5::Encode(const std::string &src) {
  temp_a_ = A;
  temp_b_ = B;
  temp_c_ = C;
  temp_d_ = D;
  str_length_ = 0;
  vector<unsigned int> rec = Padding(src);
  for (unsigned int i = 0; i < str_length_ / 16; i++) {
    unsigned int num[16];
    for (int j = 0; j < 16; j++) {
      num[j] = rec[i * 16 + j];
    }
    IterateFunc(num);
  }
  return Format(temp_a_) + Format(temp_b_) + Format(temp_c_) + Format(temp_d_);
}

void MD5::IterateFunc(unsigned int *X) {
  unsigned int a = temp_a_, b = temp_b_, c = temp_c_, d = temp_d_, rec, g, k;
  for (int i = 0; i < 64; i++) {
    if (i < 16) {
      g = F(b, c, d);
      k = i;
    } else if (i < 32) {
      g = G(b, c, d);
      k = (1 + 5 * i) % 16;
    } else if (i < 48) {
      g = H(b, c, d);
      k = (5 + 3 * i) % 16;
    } else {
      g = I(b, c, d);
      k = (7 * i) % 16;
    }
    rec = d;
    d = c;
    c = b;
    b = b + shift(a + g + X[k] + T[i], s[i]);
    a = rec;
  }
  temp_a_ += a;
  temp_b_ += b;
  temp_c_ += c;
  temp_d_ += d;
}

vector<unsigned int> MD5::Padding(const string &src) {
  unsigned int num = ((src.length() + 8) / 64) + 1;
  vector<unsigned int> rec(num * 16);
  str_length_ = num * 16;
  for (unsigned int i = 0; i < src.length(); i++) {
    rec[i >> 2] |= (int) (src[i]) << ((i % 4) * 8);
  }
  rec[src.length() >> 2] |= (0x80 << ((src.length() % 4) * 8));
  rec[rec.size() - 2] = (src.length() << 3);
  return rec;
}

string MD5::Format(unsigned int num) {
  string res;
  unsigned int base = 1 << 8;
  for (int i = 0; i < 4; i++) {
    char tmp[2];
    unsigned int b = (num >> (i * 8)) % base & 0xff;
    for (int j = 1; j >= 0; j--) {
      tmp[j] = str16[b % 16];
      b /= 16;
    }
    res += tmp;
  }
  return res;
}