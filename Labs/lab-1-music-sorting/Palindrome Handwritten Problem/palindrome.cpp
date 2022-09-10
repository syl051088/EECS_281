/* 
 * palindrome.cpp 
 * Univeristy of Michigan, Ann Arbor
 * EECS 281 Lab 1 Written.
 * SUBMIT ONLY THIS FILE TO GRADESCOPE.
 */
#include "palindrome.h"

using namespace std;

bool isPalindrome(Node* start, Node* end) {
    if (start == nullptr && end == nullptr) {
        return true;
    }
    while (start->value == end->value) {
        if(start == end || start->next == end) {
            return true;
        }
        start = start->next;
        end = end->prev;
    }

    return false;
}
