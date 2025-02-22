/* 
 * sort012.cpp 
 * Univeristy of Michigan, Ann Arbor
 * EECS 281 Lab 5 Written.
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

#include "sort012.h"

using namespace std;

void sort012(vector<int>& nums) {
  size_t start = 0;
  size_t end = nums.size() - 1;
  size_t current = 0;

  while (!nums.empty() && current <= end) {
    if (nums[current] > 1) {
      swap(nums[current], nums[end]);
      if (--end >= nums.size()) break;
    } else if (nums[current] < 1) {
      swap(nums[current], nums[start]);
      ++start;
      ++current;
    }
    if (nums[current] == 1) {
      ++current;
    }
  }
