#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>

struct LCA {
  std::vector<std::vector<int>> tree;
  std::vector<int> depth;
  std::vector<int> parent;
  std::vector<std::vector<int>> sparseTable;

  void initTree(int n, const std::vector<std::pair<int, int>>& edges) {
    tree.resize(n);
    depth.resize(n, -1);
    parent.resize(n, -1);

    for (const auto& edge : edges) {
      tree[edge.first].push_back(edge.second);
      tree[edge.second].push_back(edge.first);
    }

    std::queue<int> q;
    q.push(0);
    depth[0] = 0;

    while (!q.empty()) {
      int current = q.front();
      q.pop();

      for (int neighbor : tree[current]) {
        if (depth[neighbor] == -1) {
          depth[neighbor] = depth[current] + 1;
          parent[neighbor] = current;
          q.push(neighbor);
        }
      }
    }
  }

  void buildSparseTable(int numNodes) {
    int logN = (int)log2(numNodes) + 1;
    sparseTable.assign(numNodes, std::vector<int>(logN, -1));

    for (int i = 0; i < numNodes; ++i) {
      sparseTable[i][0] = parent[i];
    }

    for (int j = 1; (1 << j) < numNodes; ++j) {
      for (int i = 0; i < numNodes; ++i) {
        if (sparseTable[i][j - 1] != -1) {
          sparseTable[i][j] = sparseTable[sparseTable[i][j - 1]][j - 1];
        }
      }
    }
  }

  int solveWithRMQ(int u, int v) {
    if (depth[u] < depth[v])
      std::swap(u, v);

    int log = (int)log2(depth[u]);
    for (int i = log; i >= 0; --i) {
      if (depth[u] - (1 << i) >= depth[v]) {
        u = sparseTable[u][i];
      }
    }

    if (u == v)
      return u;

    for (int i = log; i >= 0; --i) {
      if (sparseTable[u][i] != sparseTable[v][i]) {
        u = sparseTable[u][i];
        v = sparseTable[v][i];
      }
    }

    return parent[u];
  }

  int calculateDistance(int u, int v) {
    int lca = solveWithRMQ(u, v);
    return depth[u] + depth[v] - 2 * depth[lca];
  }
};

int main() {
  std::ifstream inputFile("input.txt");
  int n;
  inputFile >> n;

  std::vector<std::pair<int, int>> edges(n - 1);
  for (int i = 0; i < n - 1; ++i) {
    int u, v;
    inputFile >> u >> v;
    edges[i] = {u, v};
  }

  LCA lcaSolver;
  lcaSolver.initTree(n, edges);
  lcaSolver.buildSparseTable(n);

  int q;
  inputFile >> q;

  for (int i = 0; i < q; ++i) {
    int u, v, t;
    inputFile >> u >> v >> t;

    int distance = lcaSolver.calculateDistance(u, v);
    if (distance <= t) {
      printf("%s", "Yes\n");
    } else {
      printf("%s", "No\n");
    }
  }

  inputFile.close();
  return 0;
}
