#ifndef RLE_H
#define RLE_H
#include <string>
std::string compress(const std::string& data);
std::string decompress(const std::string& compressed);
#endif