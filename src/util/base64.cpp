//
// Created by Taiga on 2023/3/24.
//

#include "util/base64.h"
#include "string"

using namespace std;
using namespace calabash;

const char Base64EncodeMap[64] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
};

const unsigned char Base64DecodeMap[256] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3E, 0xFF, 0xFF, 0xFF, 0x3F,
    0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF,
    0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
    0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
    0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

[[maybe_unused]] string Base64::Encode(const string &input) {
  string output;
  for (int i = 0; i < input.size(); i += 3) {
    output.push_back(Base64EncodeMap[input[i] >> 2]);
    size_t count = input.size() - i;
    if (count > 1) {
      output.push_back(Base64EncodeMap[(input[i] & 0x3) << 4 | input[i + 1] >> 4]);
      if (count > 2) {
        output.push_back(Base64EncodeMap[(input[i + 1] & 0xf) << 2 | input[i + 2] >> 6]);
        output.push_back(Base64EncodeMap[input[i + 2] & 0x3f]);
      }else{
        output.push_back(Base64EncodeMap[(input[i + 1] & 0xf) << 2]);
        output.push_back('=');
        break;
      }
    } else {
      output.push_back(Base64EncodeMap[(input[i] & 0x3) << 4]);
      output.push_back('=');
      output.push_back('=');
      break;
    }
  }
  return output;
}

[[maybe_unused]] string Base64::Decode(const string &input) {
  if (input.size() % 4) return {};
  string output;
  for (int i = 0; i < input.size(); i += 4) {
    unsigned char ch_0 =  Base64DecodeMap[input[i]];
    unsigned char ch_1 =  Base64DecodeMap[input[i + 1]];
    unsigned char ch_2 =  Base64DecodeMap[input[i + 2]];
    unsigned char ch_3 =  Base64DecodeMap[input[i + 3]];

    if (ch_0 == 0xFF || ch_1 == 0xFF) return {};
    output.push_back(static_cast<char>(ch_0 << 2 | ch_1 >> 4));

    if (input[i + 2] == '=') break;
    if (ch_2 == 0xFF) return {};
    output.push_back(static_cast<char>(ch_1 << 4 | ch_2 >> 2));

    if (input[i + 3] == '=')break;
    if (ch_3 == 0xFF) return {};
    output.push_back(static_cast<char>(ch_2 << 6 | ch_3));
  }
  return output;
}