#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <utility>
#include <vector>

std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
int iterations = 0;

struct Node {
  long long value;
  int priority;
  int size;
  long long sum;
  Node* left;
  Node* right;

  Node(long long val)
      : value(val)
      , priority(std::uniform_int_distribution<int>(1, 1000000000)(rng))
      , size(1)
      , sum(val)
      , left(nullptr)
      , right(nullptr) {
  }
  void update() {
    size = 1;
    sum = value;
    if (left) {
      size += left->size;
      sum += left->sum;
    }
    if (right) {
      size += right->size;
      sum += right->sum;
    }
  }
};

std::tuple<int, int, int, int> compute_indices(int x, int y) {
  int ev_start = (x % 2 == 0) ? x : x + 1;
  int ev_end = (y % 2 == 0) ? y : y - 1;
  int odd_start = (x % 2 == 1) ? x : x + 1;
  int odd_end = (y % 2 == 1) ? y : y - 1;

  int even_l = (ev_start <= y && ev_start <= ev_end) ? ev_start / 2 : -1;
  int even_r = (ev_start <= y && ev_start <= ev_end) ? ev_end / 2 : -1;
  int odd_l = (odd_start <= y && odd_start <= odd_end) ? odd_start / 2 : -1;
  int odd_r = (odd_start <= y && odd_start <= odd_end) ? odd_end / 2 : -1;

  return std::make_tuple(even_l, even_r, odd_l, odd_r);
}

std::pair<Node*, Node*> splitTreap(Node* node, int key) {
  if (!node)
    return {nullptr, nullptr};
  int left_size = node->left ? node->left->size : 0;
  if (key <= left_size) {
    auto left_split = splitTreap(node->left, key);
    node->left = left_split.second;
    node->update();
    return {left_split.first, node};
  } else {
    auto right_split = splitTreap(node->right, key - left_size - 1);
    node->right = right_split.first;
    node->update();
    return {node, right_split.second};
  }
}

Node* mergeTreap(Node* left, Node* right) {
  if (!left)
    return right;
  if (!right)
    return left;
  if (left->priority > right->priority) {
    left->right = mergeTreap(left->right, right);
    left->update();
    return left;
  } else {
    right->left = mergeTreap(left, right->left);
    right->update();
    return right;
  }
}

Node* buildTreap(const std::vector<long long>& values) {
  Node* root = nullptr;
  for (auto& val : values) {
    root = mergeTreap(root, new Node(val));
  }
  return root;
}

std::pair<std::vector<long long>, std::vector<long long>> extractEvenOdd(
    const std::vector<long long>& values
) {
  std::vector<long long> even, odd;
  for (std::vector<long long>::size_type i = 0; i < values.size(); ++i) {
    if (i % 2 == 0)
      even.push_back(values[i]);
    else
      odd.push_back(values[i]);
  }
  return {even, odd};
}

long long sumTree(Node* node) {
  return node ? node->sum : 0;
}

void inorder(Node* root, std::vector<long long>& res) {
  if (!root)
    return;
  inorder(root->left, res);
  res.push_back(root->value);
  inorder(root->right, res);
}

std::vector<long long> toArray(Node* even_tree, Node* odd_tree, int n) {
  std::vector<long long> evenElems;
  std::vector<long long> oddElems;

  inorder(even_tree, evenElems);
  inorder(odd_tree, oddElems);

  std::vector<long long> result(n, 0);

  for (int i = 0; i < (int)evenElems.size(); i++) {
    int pos = 2 * i;
    if (pos < n) {
      result[pos] = evenElems[i];
    }
  }

  for (int i = 0; i < (int)oddElems.size(); i++) {
    int pos = 2 * i + 1;
    if (pos < n) {
      result[pos] = oddElems[i];
    }
  }

  return result;
}

void deleteTree(Node* node) {
  if (!node)
    return;
  deleteTree(node->left);
  deleteTree(node->right);
  delete node;
}

std::vector<long long> processQueries(
    const std::vector<long long>& array, const std::vector<std::vector<int>>& queries
) {
  std::pair<std::vector<long long>, std::vector<long long>> even_odd = extractEvenOdd(array);
  std::vector<long long> even_values = even_odd.first;
  std::vector<long long> odd_values = even_odd.second;

  Node* even_tree = buildTreap(even_values);
  Node* odd_tree = buildTreap(odd_values);

  std::vector<long long> results;

  for (auto& query : queries) {
    if (query[0] == 1) {
      int orig_x = query[1];
      int x = orig_x - 1;
      int y = query[2] - 1;

      int even_l, even_r, odd_l, odd_r;
      std::tie(even_l, even_r, odd_l, odd_r) = compute_indices(x, y);

      Node* even_left = nullptr;
      Node* even_mid = nullptr;
      Node* even_right = nullptr;
      if (even_l != -1 && even_r != -1) {
        auto split_e_left_mid = splitTreap(even_tree, even_l);
        even_left = split_e_left_mid.first;
        Node* even_mid_right = split_e_left_mid.second;

        auto split_e_mid_right = splitTreap(even_mid_right, even_r - even_l + 1);
        even_mid = split_e_mid_right.first;
        even_right = split_e_mid_right.second;
      } else {
        even_left = even_tree;
        even_right = nullptr;
      }

      Node* odd_left = nullptr;
      Node* odd_mid = nullptr;
      Node* odd_right = nullptr;
      if (odd_l != -1 && odd_r != -1) {
        auto split_o_left_mid = splitTreap(odd_tree, odd_l);
        odd_left = split_o_left_mid.first;
        Node* odd_mid_right = split_o_left_mid.second;

        auto split_o_mid_right = splitTreap(odd_mid_right, odd_r - odd_l + 1);
        odd_mid = split_o_mid_right.first;
        odd_right = split_o_mid_right.second;
      } else {
        odd_left = odd_tree;
        odd_right = nullptr;
      }
      even_tree = mergeTreap(mergeTreap(even_left, odd_mid), even_right);
      odd_tree = mergeTreap(mergeTreap(odd_left, even_mid), odd_right);

    } else if (query[0] == 2) {
      int a = query[1] - 1;
      int b = query[2] - 1;
      int even_l, even_r, odd_l, odd_r;
      std::tie(even_l, even_r, odd_l, odd_r) = compute_indices(a, b);

      Node* even_left = nullptr;
      Node* even_mid = nullptr;
      Node* even_right = nullptr;

      if (even_l != -1 && even_r != -1) {
        auto split_e_left_mid = splitTreap(even_tree, even_l);
        even_left = split_e_left_mid.first;
        Node* even_mid_right = split_e_left_mid.second;

        auto split_e_mid_right = splitTreap(even_mid_right, even_r - even_l + 1);
        even_mid = split_e_mid_right.first;
        even_right = split_e_mid_right.second;
      } else {
        even_left = even_tree;
        even_right = nullptr;
      }

      Node* odd_left = nullptr;
      Node* odd_mid = nullptr;
      Node* odd_right = nullptr;

      if (odd_l != -1 && odd_r != -1) {
        auto split_o_left_mid = splitTreap(odd_tree, odd_l);
        odd_left = split_o_left_mid.first;
        Node* odd_mid_right = split_o_left_mid.second;

        auto split_o_mid_right = splitTreap(odd_mid_right, odd_r - odd_l + 1);
        odd_mid = split_o_mid_right.first;
        odd_right = split_o_mid_right.second;
      } else {
        odd_left = odd_tree;
        odd_right = nullptr;
      }

      long long total_sum = sumTree(even_mid) + sumTree(odd_mid);
      results.push_back(total_sum);
      even_tree = mergeTreap(mergeTreap(even_left, even_mid), even_right);
      odd_tree = mergeTreap(mergeTreap(odd_left, odd_mid), odd_right);
    }
  }
  deleteTree(even_tree);
  deleteTree(odd_tree);
  return results;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(0);
  std::ifstream infile("input.txt");
  int test_case = 1;
  while (true) {
    int n, r;
    infile >> n >> r;
    if (n == 0 && r == 0)
      break;
    std::vector<long long> array(n);
    for (int i = 0; i < n; ++i) {
      infile >> array[i];
    }
    std::vector<std::vector<int>> queries;
    for (int i = 0; i < r; ++i) {
      int q_type, a, b;
      infile >> q_type >> a >> b;
      queries.push_back({q_type, a, b});
    }
    std::vector<long long> results = processQueries(array, queries);
    Node* even_tree = nullptr;
    Node* odd_tree = nullptr;
    std::cout << "Suite " << test_case << ":\n";
    for (auto& res : results) {
      std::cout << res << "\n";
    }
    std::cout << "\n";
    test_case++;
    deleteTree(even_tree);
    deleteTree(odd_tree);
  }

  return 0;
}
