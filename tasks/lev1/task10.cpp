#include <algorithm>
#include <iostream>
#include <vector>

std::vector<int> setUnion(const std::vector<int> &a,
                          const std::vector<int> &b) {
  std::vector<int> result;
  std::set_union(a.begin(), a.end(), b.begin(), b.end(),
                 std::back_inserter(result));
  return result;
}

std::vector<int> setIntersection(const std::vector<int> &a,
                                 const std::vector<int> &b) {
  std::vector<int> result;
  std::set_intersection(a.begin(), a.end(), b.begin(), b.end(),
                        std::back_inserter(result));
  return result;
}

std::vector<int> setDifference(const std::vector<int> &a,
                               const std::vector<int> &b) {
  std::vector<int> result;
  std::set_difference(a.begin(), a.end(), b.begin(), b.end(),
                      std::back_inserter(result));
  return result;
}

int main() {
  std::vector<int> set1 = {1, 2, 3, 4, 5};
  std::vector<int> set2 = {3, 4, 5, 6, 7};

  std::sort(set1.begin(), set1.end());
  std::sort(set2.begin(), set2.end());

  auto u = setUnion(set1, set2);
  auto i = setIntersection(set1, set2);
  auto d = setDifference(set1, set2);

  auto print = [](const std::vector<int> &v, const std::string &name) {
    std::cout << name << ": ";
    for (int x : v)
      std::cout << x << " ";
    std::cout << "\n";
  };

  print(set1, "Множество A");
  print(set2, "Множество B");
  print(u, "Объединение");
  print(i, "Пересечение");
  print(d, "Разность A\\B");

  return 0;
}