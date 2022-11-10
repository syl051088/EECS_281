/*
 * replace_words.cpp
 * Univeristy of Michigan, Ann Arbor
 * EECS 281 Lab 7 Written.
 * SUBMIT ONLY THIS FILE TO GRADESCOPE.
 */

// Common #includes for convience.
// No need to use them.
// Remove any of them if you want.
#include <algorithm>
#include <cassert>
#include <deque>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <math.h>
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

#include "replace_words.h"

using namespace std;

vector<string> replace_words(const vector<string>& prefixes,
                             const vector<string>& sentence) {
  vector<string> v;
  v.reserve(sentence.size());
  unordered_set<string> s;
  for (string i : prefixes) {
    s.insert(i);
  }
  for (size_t i = 0; i < sentence.size(); ++i) {
    string next = sentence[i];
    for (size_t j = 1; j <= next.size(); ++j) {
      auto it = s.find(next.substr(0, j));
      if (it != s.end()) {
        next = *it;
        break;
      }
    }
    v.push_back(next);
  }
  return v;
}

int main() {
  vector<string> prefixes = {"cat", "bat", "rat"};
  vector<string> sentence = {"the", "cattle", "was", "rattled", "by", "the", "battery"};
  replace_words(prefixes, sentence);
  return 0;
}