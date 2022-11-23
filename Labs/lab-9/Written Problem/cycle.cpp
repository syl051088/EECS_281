/* 
 * cycle.cpp 
 * Univeristy of Michigan, Ann Arbor
 * EECS 281 Lab 9 Written.
 * SUBMIT ONLY THIS FILE TO GRADESCOPE.
 */

// Common #includes for convience.
// No need to use them. 
// Remove any of them if you want.
#include <algorithm>
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
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "cycle.h"

using namespace std;

struct Adj {
  int val, src;
};

bool is_graph_cyclic(const vector<vector<int>> &adj_list) {
  if (adj_list.size() == 0) {
    return false;
  }
  vector<bool> isvisited;
  isvisited.resize(adj_list.size(), false);
  isvisited[0] = true;
  deque<Adj> bst;
  bst.push_back(Adj{0, -1});
  while (!bst.empty()) {
    Adj current = bst.front();
    bst.pop_front();
    for (int i : adj_list[static_cast<size_t>(current.val)]) {
      if (!isvisited[static_cast<size_t>(i)]) {
        bst.push_back(Adj{i, current.val});
        isvisited[i] = true;
      } else if (i != current.src) {
        return true;
      }
    }
  }

  return false;
}
