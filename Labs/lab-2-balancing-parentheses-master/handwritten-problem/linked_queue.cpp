/*
 * linked_queue.cpp
 * Univeristy of Michigan, Ann Arbor
 * EECS 281 Lab 2 Written.
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

#include "linked_queue.h"

using namespace std;

template <typename T>
T LinkedQueue<T>::front() const {
  assert(!empty());
  return head->value;
}

template <typename T>
void LinkedQueue<T>::pop() {
  assert(!empty());
  Node<T>* temp = head;
  if (size() == 1) {
    head = nullptr;
    tail = nullptr;
  } else {
    head = head->next;
  }
  delete temp;
  --count;
}

template <typename T>
void LinkedQueue<T>::push(T x) {
  Node<T>* temp = new Node<T>;
  temp->value = x;
  temp->next = nullptr;
  if (empty()) {
    head = temp;
    tail = temp;
  } else {
    tail->next = temp;
    tail = temp;
  }

  ++count;
}

template <typename T>
LinkedQueue<T>::~LinkedQueue() {
  while (!empty()) {
    pop();
  }
}
