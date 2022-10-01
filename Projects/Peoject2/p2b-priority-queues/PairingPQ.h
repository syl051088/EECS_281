// Project identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043

#ifndef PAIRINGPQ_H
#define PAIRINGPQ_H

#include "Eecs281PQ.h"
#include <deque>
#include <utility>

// A specialized version of the priority queue ADT implemented as a pairing
// heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class PairingPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Each node within the pairing heap
    class Node {
        public:
            explicit Node(const TYPE &val)
                : elt{ val }, child{ nullptr }, sibling{ nullptr }, parent{ nullptr }
            {}

            // Description: Allows access to the element at that Node's
            //              position. There are two versions, getElt() and a
            //              dereference operator, use whichever one seems
            //              more natural to you.
            // Runtime: O(1) - this has been provided for you.
            const TYPE &getElt() const { return elt; }
            const TYPE &operator*() const { return elt; }

            // The following line allows you to access any private data
            // members of this Node class from within the PairingPQ class.
            // (ie: myNode.elt is a legal statement in PairingPQ's add_node()
            // function).
            friend PairingPQ;

        private:
            TYPE elt;
            Node *child;
            Node *sibling;
            Node *parent;
    }; // Node


    // Description: Construct an empty pairing heap with an optional
    //              comparison functor.
    // Runtime: O(1)
    explicit PairingPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp }, root{ nullptr }, numNodes{ 0 } {
    } // PairingPQ()


    // Description: Construct a pairing heap out of an iterator range with an
    //              optional comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    template<typename InputIterator>
    PairingPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp }, root { nullptr }, numNodes { 0 } {
            while (start != end) {
                push(*start);
                ++start;
            }
    } // PairingPQ()


    // Description: Copy constructor.
    // Runtime: O(n)
    PairingPQ(const PairingPQ &other) :
        BaseClass{ other.compare }, root{nullptr}, numNodes{0} {
        std::deque<Node*> dq;
        dq.push_back(other.root);
        while (!dq.empty()) {
            Node* current = dq.back();
            dq.pop_back();
            if (current->child != nullptr) dq.push_back(current->child);
            if (current->sibling != nullptr) dq.push_back(current->sibling);
            push(current->elt);
        }
    } // PairingPQ()


    // Description: Copy assignment operator.
    // Runtime: O(n)
    PairingPQ &operator=(const PairingPQ &rhs) {
        PairingPQ temp(rhs);

        std::swap(root, temp.root);
        std::swap(numNodes, temp.numNodes);

        return *this;
    } // operator=()


    // Description: Destructor
    // Runtime: O(n)
    ~PairingPQ() {
        if (numNodes != 0) {
            std::deque<Node*> dq;
            dq.push_back(root);
            while (!dq.empty()) {
                Node* current = dq.front();
                dq.pop_front();
                if (current->child != nullptr) dq.push_back(current->child);
                if (current->sibling != nullptr) dq.push_back(current->sibling);
                delete current;
                current = nullptr;
            }
        }
    } // ~PairingPQ()


    // Description: Assumes that all elements inside the pairing heap are out
    //              of order and 'rebuilds' the pairing heap by fixing the
    //              pairing heap invariant. You CANNOT delete 'old' nodes
    //              and create new ones!
    // Runtime: O(n)
    virtual void updatePriorities() {
        if (numNodes == 0 || numNodes == 1) {
            return;
        }
        std::deque<Node*> dq;
        dq.push_back(root->child);
        root->child = nullptr;

        while (!dq.empty()) {
            Node* current = dq.back();
            dq.pop_back();
            if (current->child != nullptr) dq.push_back(current->child);
            if (current->sibling != nullptr) dq.push_back(current->sibling);
            current->parent = nullptr;
            current->sibling = nullptr;
            current->child = nullptr;

            root = meld(current, root);
        }
    } // updatePriorities()


    // Description: Add a new element to the pairing heap. This is already
    //              done. You should implement push functionality entirely in
    //              the addNode() function, and this function calls
    //              addNode().
    // Runtime: O(1)
    virtual void push(const TYPE &val) {
        addNode(val);
    } // push()


    // Description: Remove the most extreme (defined by 'compare') element
    //              from the pairing heap.
    // Note: We will not run tests on your code that would require it to pop
    //       an element when the pairing heap is empty. Though you are
    //       welcome to if you are familiar with them, you do not need to use
    //       exceptions in this project.
    // Runtime: Amortized O(log(n))
    virtual void pop() {
        std::deque<Node*> dq;
        Node* start = root->child;
        delete root;
        root = nullptr;
        while (start != nullptr) {
            dq.push_back(start);
            start = start->sibling;
        }

        while (dq.size() > 1) {
            Node* temp1 = dq.front();
            dq.pop_front();
            Node* temp2 = dq.front();
            dq.pop_front();
            temp1->parent = nullptr;
            temp1->sibling = nullptr;
            temp2->parent = nullptr;
            temp2->sibling = nullptr;
            meld(temp1, temp2);

            temp1->parent == nullptr? dq.push_back(temp1) : dq.push_back(temp2);
        }
        if (dq.size() == 1) {
            Node* temp = dq.back();
            dq.pop_back();
            root = temp;
        }
        --numNodes;
    } // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the pairing heap. This should be a reference for speed.
    //              It MUST be const because we cannot allow it to be
    //              modified, as that might make it no longer be the most
    //              extreme element.
    // Runtime: O(1)
    virtual const TYPE &top() const {
        return root->elt;
    } // top()


    // Description: Get the number of elements in the pairing heap.
    // Runtime: O(1)
    virtual std::size_t size() const {
        return numNodes;
    } // size()

    // Description: Return true if the pairing heap is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        return numNodes == 0;
    } // empty()


    // Description: Updates the priority of an element already in the pairing
    //              heap by replacing the element refered to by the Node with
    //              new_value. Must maintain pairing heap invariants.
    //
    // PRECONDITION: The new priority, given by 'new_value' must be more
    //               extreme (as defined by comp) than the old priority.
    //
    // Runtime: As discussed in reading material.
    void updateElt(Node* node, const TYPE &new_value) {
        node->elt = new_value;
        Node* parent = node->parent;
        if (parent && this->compare(parent->elt, node->elt)) {
            Node* begin = parent->child;
            if (begin == node) {
                parent->child = node->sibling;
            } else {
                while (begin->sibling != node) begin = begin->sibling;
                begin->sibling = node->sibling;
            }
            
            node->parent = nullptr;
            node->sibling = nullptr;

            root = meld(node, root);
        }
    } // updateElt()


    // Description: Add a new element to the pairing heap. Returns a Node*
    //              corresponding to the newly added element.
    // NOTE: Whenever you create a node, and thus return a Node *, you must
    //       be sure to never move or copy/delete that node in the future,
    //       until it is eliminated by the user calling pop(). Remember this
    //       when you implement updateElt() and updatePriorities().
    // Runtime: O(1)
    Node* addNode(const TYPE &val) {
        Node* temp = new Node(val);
        root = numNodes == 0? temp : meld(temp, root);
        ++numNodes;
        return temp;
    } // addNode()


private:
    Node* meld(Node* n1, Node* n2) {
        if (this->compare(n1->elt, n2->elt)) {
            n1->sibling = n2->child;
            n2->child = n1;
            n1->parent = n2;
            return n2;
        }
        n2->sibling = n1->child;
        n1->child = n2;
        n2->parent = n1;
        return n1;
    }

    Node *root;
    std::size_t numNodes;
};


#endif // PAIRINGPQ_H
