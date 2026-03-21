#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<int> buildPrefixFunction(const string &pattern) {
  int m = pattern.length();
  vector<int> pi(m, 0);
  int k = 0;
  for (int q = 1; q < m; ++q) {
    while (k > 0 && pattern[k] != pattern[q]) {
      k = pi[k - 1];
    }
    if (pattern[k] == pattern[q]) {
      ++k;
    }
    pi[q] = k;
  }
  return pi;
}

vector<int> kmpSearch(const string &text, const string &pattern) {
  vector<int> result;
  if (pattern.empty())
    return result;
  vector<int> pi = buildPrefixFunction(pattern);
  int n = text.length();
  int m = pattern.length();
  int q = 0;
  for (int i = 0; i < n; ++i) {
    while (q > 0 && pattern[q] != text[i]) {
      q = pi[q - 1];
    }
    if (pattern[q] == text[i]) {
      ++q;
    }
    if (q == m) {
      result.push_back(i - m + 1);
      q = pi[q - 1];
    }
  }
  return result;
}

int main() {
  string text = "ABABDABACDABABCABAB";
  string pattern = "ABABCABAB";

  vector<int> matches = kmpSearch(text, pattern);
  cout << "Текст: " << text << "\n";
  cout << "Образец: " << pattern << "\n";
  cout << "Найденные позиции: ";
  for (int pos : matches) {
    cout << pos << " ";
  }
  cout << "\n";

  text = "abcabcabc";
  pattern = "abc";
  matches = kmpSearch(text, pattern);
  cout << "\nТекст: " << text << ", образец: " << pattern << "\n";
  cout << "Позиции: ";
  for (int pos : matches)
    cout << pos << " ";
  cout << "\n";

  return 0;
}