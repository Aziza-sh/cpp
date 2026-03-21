#include <climits>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

typedef pair<int, int> pii;

vector<int> dijkstra(int start, int n, vector<vector<pii>> &graph) {
  vector<int> dist(n, INT_MAX);
  dist[start] = 0;
  priority_queue<pii, vector<pii>, greater<pii>> pq;
  pq.push({0, start});

  while (!pq.empty()) {
    int u = pq.top().second;
    int d = pq.top().first;
    pq.pop();

    if (d > dist[u])
      continue;

    for (auto &edge : graph[u]) {
      int v = edge.first;
      int w = edge.second;
      if (dist[u] + w < dist[v]) {
        dist[v] = dist[u] + w;
        pq.push({dist[v], v});
      }
    }
  }
  return dist;
}

int main() {
  int n = 5;
  vector<vector<pii>> graph(n);

  graph[0].push_back({1, 4});
  graph[0].push_back({2, 2});
  graph[1].push_back({2, 1});
  graph[1].push_back({3, 5});
  graph[2].push_back({3, 8});
  graph[2].push_back({4, 10});
  graph[3].push_back({4, 2});

  int start = 0;
  vector<int> distances = dijkstra(start, n, graph);

  cout << "Кратчайшие расстояния от вершины " << start << ":\n";
  for (int i = 0; i < n; ++i) {
    cout << "до " << i << ": " << distances[i] << "\n";
  }
  return 0;
}