#include <cmath>
#include <iostream>
#include <vector>
std::vector<unsigned int> arr;
std::vector<unsigned int> zerosInBlock;
unsigned int blockSize;
void initialize(const std::vector<unsigned int> &input) {
  unsigned int n = input.size();
  arr = input;
  blockSize = static_cast<unsigned int>(std::sqrt(n));
  zerosInBlock.resize((n + blockSize - 1) / blockSize, 0);
  for (unsigned int i = 0; i < n; ++i) {
    if (arr[i] == 0) {
      zerosInBlock[i / blockSize]++;
    }
  }
}
void update(unsigned int idx, unsigned int value) {
  unsigned int blockIndex = idx / blockSize;
  if (arr[idx] == 0)
    zerosInBlock[blockIndex]--;
  if (value == 0)
    zerosInBlock[blockIndex]++;
  arr[idx] = value;
}

int findZero(unsigned int L, unsigned int R, unsigned int k) {
  L--;
  R--;
  unsigned int startBlock = L / blockSize;
  unsigned int endBlock = R / blockSize;
  unsigned int zeros = 0;
  if (startBlock == endBlock) {
    for (unsigned int i = L; i <= R; ++i) {
      if (arr[i] == 0 && ++zeros == k)
        return i + 1;
    }
    return -1;
  }
  for (unsigned int i = L; i < (startBlock + 1) * blockSize; ++i) {
    if (arr[i] == 0 && ++zeros == k)
      return i + 1;
  }
  for (unsigned int b = startBlock + 1; b < endBlock; ++b) {
    if (zeros + zerosInBlock[b] >= k) {
      for (unsigned int i = b * blockSize; i < (b + 1) * blockSize; ++i) {
        if (arr[i] == 0 && ++zeros == k)
          return i + 1;
      }
    }
    zeros += zerosInBlock[b];
  }

  for (unsigned int i = endBlock * blockSize; i <= R; ++i) {
    if (arr[i] == 0 && ++zeros == k)
      return i + 1;
  }
  return -1;
}
int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  unsigned int n, q;
  std::cin >> n;
  std::vector<unsigned int> input(n);
  for (unsigned int i = 0; i < n; ++i) {
    std::cin >> input[i];
  }
  initialize(input);
  std::cin >> q;
  for (unsigned int i = 0; i < q; ++i) {
    std::string type;
    std::cin >> type;
    if (type == "u") {
      unsigned int idx, value;

      std::cin >> idx >> value;
      update(idx - 1, value);
    } else if (type == "s") {
      unsigned int L, R, k;
      std::cin >> L >> R >> k;
      std::cout << findZero(L, R, k) << '\n';
    }
  }
  return 0;
}