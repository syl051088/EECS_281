// Project identifier: 0E04A31E0D60C01986ACB20081C9D8722A1899B6

#ifndef BINARYPQ_H
#define BINARYPQ_H


#include <algorithm>
#include <iostream>
#include "Eecs281PQ.h"
using std::size_t;
using std::cout;
using std::endl;

// A specialized version of the priority queue ADT implemented as a binary
// heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class BinaryPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Description: Construct an empty PQ with an optional comparison functor.
    // Runtime: O(1)
    explicit BinaryPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {
        // TODO: Implement this function, or verify that it is already done
    } // BinaryPQ


    // Description: Construct a PQ out of an iterator range with an optional
    //              comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    template<typename InputIterator>
    BinaryPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp }, data {start, end} {
            updatePriorities();
    } // BinaryPQ


    // Description: Destructor doesn't need any code, the data vector will
    //              be destroyed automatically.
    virtual ~BinaryPQ() {
    } // ~BinaryPQ()


    // Description: Assumes that all elements inside the heap are out of
    //              order and 'rebuilds' the heap by fixing the heap
    //              invariant.
    // Runtime: O(n)
    virtual void updatePriorities() {
        for (size_t i = data.size(); i > 0; --i) {
            fixDown(i);
        }
    } // updatePriorities()


    // Description: Add a new element to the PQ.
    // Runtime: O(log(n))
    virtual void push(const TYPE &val) {
        data.push_back(val);
        fixUp(data.size());
    } // push()


    // Description: Remove the most extreme (defined by 'compare') element
    //              from the PQ.
    // Note: We will not run tests on your code that would require it to pop
    //       an element when the PQ is empty. Though you are welcome to if
    //       you are familiar with them, you do not need to use exceptions in
    //       this project.
    // Runtime: O(log(n))
    virtual void pop() {
        data[0] = data.back();
        data.pop_back();
        fixDown(1);
    } // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the PQ. This should be a reference for speed. It MUST
    //              be const because we cannot allow it to be modified, as
    //              that might make it no longer be the most extreme element.
    // Runtime: O(1)
    virtual const TYPE &top() const {
        return data[0];
    } // top()


    // Description: Get the number of elements in the PQ.
    // Runtime: O(1)
    virtual std::size_t size() const {
        return data.size();
    } // size()


    // Description: Return true if the PQ is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        return data.empty();
    } // empty()


private:
    // Note: This vector *must* be used for your PQ implementation.
    std::vector<TYPE> data;
    // NOTE: You are not allowed to add any member variables. You don't need
    //       a "heapSize", since you can call your own size() member
    //       function, or check data.size().

    // TODO: Add any additional member functions you require here. For
    //       instance, you might add fixUp() and fixDown().
    virtual void fixUp(size_t k) {
        while (k > 1 && this->compare(getElement(k / 2), getElement(k))) {
            std::swap(getElement(k), getElement(k / 2));

            k = k / 2;
        }
    }

    virtual void fixDown(size_t k) {
        while (2 * k <= data.size()) {
            size_t j = k * 2;
            if (j < data.size() && this->compare(getElement(j), getElement(j + 1))) ++j;
            if (!this->compare(getElement(k), getElement(j))) break;
            std::swap(getElement(k), getElement(j));
            k = j;
        }
    }

    // Translate 1-based indexing into a 0-based vector
    TYPE &getElement(std::size_t i) {
      return data[i - 1];
    }  // getElement()

    const TYPE &getElement(std::size_t i) const {
      return data[i - 1];
    }  // getElement()
}; // BinaryPQ


#endif // BINARYPQ_H
