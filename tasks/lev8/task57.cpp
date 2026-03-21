#include <iostream>
#include <vector>

using namespace std;

class GraphMatrix {
private:
  int n;
  vector<vector<int>> adj;

public:
  GraphMatrix(int vertices)
      : n(vertices), adj(vertices, vector<int>(vertices, 0)) {}

  void addEdge(int u, int v, int weight = 1) {
    adj[u][v] = weight;
    adj[v][u] = weight;
  }

  void print() {
    cout << "Матрица смежности:\n";
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        cout << adj[i][j] << " ";
      }
      cout << "\n";
    }
  }
};

class GraphList {
private:
  int n;
  vector<vector<pair<int, int>>> adj;

public:
  GraphList(int vertices) : n(vertices), adj(vertices) {}

  void addEdge(int u, int v, int weight = 1) {
    adj[u].push_back({v, weight});
    adj[v].push_back({u, weight});
  }

  void print() {
    cout << "Список смежности:\n";
    for (int i = 0; i < n; ++i) {
      cout << i << ": ";
      for (auto &edge : adj[i]) {
        cout << "(" << edge.first << ", " << edge.second << ") ";
      }
      cout << "\n";
    }
  }
};

int main() {
  int vertices = 5;
  GraphMatrix gm(vertices);
  gm.addEdge(0, 1);
  gm.addEdge(0, 2);
  gm.addEdge(1, 3);
  gm.addEdge(3, 4);
  gm.print();

  GraphList gl(vertices);
  gl.addEdge(0, 1);
  gl.addEdge(0, 2);
  gl.addEdge(1, 3);
  gl.addEdge(3, 4);
  gl.print();

  return 0;
}