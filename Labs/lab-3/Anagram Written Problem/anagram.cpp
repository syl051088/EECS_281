/*
 * anagram.cpp
 * Univeristy of Michigan, Ann Arbor
 * EECS 281 Lab 3 Written.
 * SUBMIT ONLY THIS FILE TO GRADESCOPE.
 */

// Common #includes for convenience.
// No need to use them.
// Remove any of them if you want.
#include <algorithm>
#include <cassert>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "anagram.h"

using namespace std;

bool isAnagram(const string &s1, const string &s2) {
  unordered_map<char, int> umap;
  for (char c : s1) {
    if (c == ' ') {
      continue;
    }
    if (umap.find(c) == umap.end()) {
      umap[c] = 1;
    } else {
      ++umap[c];
    }
  }

  for (char c : s2) {
    if (c == ' ') {
      continue;
    }

    if (umap.find(c) == umap.end()) {
      return false;
    }

    --umap[c];
  }

  for (const auto &i : umap) {
    if (i.second != 0) {
      return false;
    }
  }
  return true;
}

// int main() {
//   string s1 = "anagram";
//   string s2 = "nagaram";

//   s1 = "i love eecs";
//   s2 = "i scole ve e";

//   s1 = "anagrams";
//   s2 = "anagrams anagrams";

//   // s1 = "cats";
//   // s2 = "cat";

//   if (isAnagram(s1, s2)) {
//     cout << "true\n";
//   } else {
//     cout << "false\n";
//   }

//   return 0;
// }
