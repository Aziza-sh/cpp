#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int knapsack(int capacity, const vector<int> &weights,
             const vector<int> &values) {
  int n = weights.size();
  vector<vector<int>> dp(n + 1, vector<int>(capacity + 1, 0));

  for (int i = 1; i <= n; ++i) {
    for (int w = 1; w <= capacity; ++w) {
      if (weights[i - 1] <= w) {
        dp[i][w] =
            max(dp[i - 1][w], dp[i - 1][w - weights[i - 1]] + values[i - 1]);
      } else {
        dp[i][w] = dp[i - 1][w];
      }
    }
  }
  return dp[n][capacity];
}

int knapsackOptimized(int capacity, const vector<int> &weights,
                      const vector<int> &values) {
  int n = weights.size();
  vector<int> dp(capacity + 1, 0);
  for (int i = 0; i < n; ++i) {
    for (int w = capacity; w >= weights[i]; --w) {
      dp[w] = max(dp[w], dp[w - weights[i]] + values[i]);
    }
  }
  return dp[capacity];
}

int main() {
  vector<int> weights = {2, 3, 4, 5};
  vector<int> values = {3, 4, 5, 6};
  int capacity = 5;

  int maxValue = knapsack(capacity, weights, values);
  cout << "Максимальная ценность (DP 2D): " << maxValue << "\n";

  int maxValueOpt = knapsackOptimized(capacity, weights, values);
  cout << "Максимальная ценность (DP 1D): " << maxValueOpt << "\n";

  return 0;
}