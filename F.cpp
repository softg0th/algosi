#include <iostream>
#include <string>
#include <vector>

unsigned int currentBroad = 0;

void computeLPSArray(std::string pattern, std::vector<unsigned int>& lps, unsigned int m) {
  unsigned int suffixLen = 0;
  lps[0] = 0;
  unsigned int i = 1;
  while (i < m) {
    if (pattern[i] == pattern[suffixLen]) {
      suffixLen++;
      lps[i] = suffixLen;
      i++;
    } else {
      if (suffixLen != 0) {
        suffixLen = lps[suffixLen - 1];
      } else {
        lps[i] = 0;
        i++;
      }
    }
  }
}

bool processKMPSearch(std::string txt, std::string pat, unsigned int& currentPosition) {
  unsigned int m = pat.size();
  unsigned int n = txt.size();
  std::vector<unsigned int> lps(m);
  computeLPSArray(pat, lps, m);
  unsigned int i = currentPosition;
  unsigned int j = 0;

  while (i < n) {
    if (pat[j] == txt[i]) {
      j++;
      i++;
    }
    if (j == m) {
      currentPosition = i;
      return true;
    } else if (i < n && pat[j] != txt[i]) {
      if (j != 0) {
        j = lps[j - 1];
      } else {
        i++;
      }
    }
  }
  return false;
}

int main() {
  unsigned int n, m;
  std::string text;
  bool isMatch = false;
  std::cin >> n >> m;
  std::cin >> text;
  unsigned int currentPosition = 0;

  for (unsigned int i = 0; i < m; ++i) {
    std::string word;
    std::cin >> word;

    isMatch = processKMPSearch(text, word, currentPosition);
    if (!isMatch) {
      std::cout << "NO" << std::endl;
      return 0;
    }

    if (i < m - 1 && currentPosition < n) {
      currentPosition++;
    }
  }

  std::cout << "YES" << std::endl;
  return 0;
}
