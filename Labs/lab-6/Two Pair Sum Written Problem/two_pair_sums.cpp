/* 
 * two_pair_sums.cpp 
 * Univeristy of Michigan, Ann Arbor
 * EECS 281 Lab 6 Written.
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

#include "two_pair_sums.h"

using namespace std;

void two_pair_sums(const vector<int>& nums, ostream& os) {
  unordered_map<int, vector<pair<int, int>>> m;
  for (size_t i = 0; i < nums.size(); ++i) {
    for (size_t j = i + 1; j < nums.size(); ++j) {
      m[nums[i]+nums[j]].push_back(make_pair(nums[i], nums[j])); 
    }
  }
  for (auto & p : m) {
    if (p.second.size() == 2) {
      os << "(" << p.second[0].first << ", " << p.second[0].second << ") " 
      << "and (" << p.second[1].first << ", " << p.second[1].second << ")" << endl;
     }
  }
}
