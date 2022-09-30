/* 
 * join_ropes.cpp 
 * Univeristy of Michigan, Ann Arbor
 * EECS 281 Lab 4 Written.
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

#include "join_ropes.h"

using namespace std;

int join_ropes(const vector<int>& rope_lengths) {
  priority_queue<int, vector<int>, greater<int>> ropePQ(rope_lengths.begin(), rope_lengths.end());
  int cost = 0;
  while (ropePQ.size() > 1) {
    int temp = ropePQ.top();
    ropePQ.pop();
    temp += ropePQ.top();
    ropePQ.pop();
    cost += temp;
    ropePQ.push(temp);
  }
  return cost;
}
