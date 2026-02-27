#include "rle.h"
#include <cctype>
#include <sstream>
#include <stdexcept>
using namespace std;

string compress(const string &data) {
  if (data.empty())
    return "";
  stringstream res;
  int count = 1;
  for (size_t i = 1; i <= data.size(); i++) {
    if (i == data.size() || data[i] != data[i - 1]) {
      res << data[i - 1] << count;
      count = 1;
    } else {
      count++;
    }
  }
  return res.str();
}

string decompress(const string &comp) {
  if (comp.empty())
    return "";
  stringstream res;
  size_t i = 0;
  while (i < comp.size()) {
    char c = comp[i++];
    if (!isalpha(c) && c != ' ') {
      throw invalid_argument("Ожидался символ (буква)");
    }
    int count = 0;
    while (i < comp.size() && isdigit(comp[i])) {
      count = count * 10 + (comp[i] - '0');
      i++;
    }
    if (count == 0) {
      throw invalid_argument("После символа должно быть положительное число");
    }
    for (int j = 0; j < count; j++) {
      res << c;
    }
  }
  return res.str();
}
