#include <algorithm>
#include <iostream>
#include <stack>
#include <vector>

struct TrieNode {
  TrieNode* children[2] = {nullptr, nullptr};
};

class Trie {
public:
  Trie() {
    node_pool.resize(32 * 200000);
    pool_index = 0;
    root = allocateNode();
  }

  void insert(int num) {
    TrieNode* node = root;
    for (int i = 31; i >= 0; --i) {
      int bit = (num >> i) & 1;
      if (!node->children[bit]) {
        node->children[bit] = allocateNode();
      }
      node = node->children[bit];
    }
  }

  int findMaxXor(int num) {
    TrieNode* node = root;
    int max_xor = 0;
    for (int i = 31; i >= 0; --i) {
      int bit = (num >> i) & 1;
      int opposite_bit = 1 - bit;
      if (node->children[opposite_bit]) {
        max_xor = (max_xor << 1) | 1;
        node = node->children[opposite_bit];
      } else {
        max_xor = (max_xor << 1);
        node = node->children[bit];
      }
    }
    return max_xor;
  }

private:
  TrieNode* root;
  std::vector<TrieNode> node_pool;
  int pool_index;

  TrieNode* allocateNode() {
    return &node_pool[pool_index++];
  }
};

int maxDiscount(int n, int f, std::vector<std::tuple<int, int, int>>& edges) {
  std::vector<std::vector<std::pair<int, int>>> graph(n + 1);
  for (auto& [u, v, w] : edges) {
    graph[u].emplace_back(v, w);
    graph[v].emplace_back(u, w);
  }

  std::vector<int> xor_to_f(n + 1, -1);
  xor_to_f[f] = 0;
  std::stack<std::pair<int, int>> stk;
  stk.push({f, -1});

  while (!stk.empty()) {
    auto [node, parent] = stk.top();
    stk.pop();
    for (auto& [neighbor, weight] : graph[node]) {
      if (neighbor != parent) {
        xor_to_f[neighbor] = xor_to_f[node] ^ weight;
        stk.push({neighbor, node});
      }
    }
  }

  Trie trie;
  for (int i = 1; i <= n; ++i) {
    if (i != f) {
      trie.insert(xor_to_f[i]);
    }
  }

  int max_xor = 0;
  for (int i = 1; i <= n; ++i) {
    if (i != f) {
      max_xor = std::max(max_xor, trie.findMaxXor(xor_to_f[i]));
    }
  }

  return max_xor;
}

int main() {
  int n, f;
  std::cin >> n >> f;

  std::vector<std::tuple<int, int, int>> edges;
  for (int i = 1; i < n; ++i) {
    int u, v, w;
    std::cin >> u >> v >> w;
    edges.push_back({u, v, w});
  }

  std::cout << maxDiscount(n, f, edges) << std::endl;
  return 0;
}
