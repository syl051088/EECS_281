#ifndef HASHTABLE_H
#define HASHTABLE_H

// Project Identifier: 2C4A3C53CD5AD45A7BEA3AE5356A3B1622C9D04B

// INSTRUCTIONS:
// fill out the methods in the class below.

// You may assume that the key and value types can be copied and have default
// constructors.

// You can also assume that the key type has (==) and (!=) methods.

// You may assume that Hasher is a functor type with a
// size_t operator()(const Key&) overload.

// The key/value types aren't guaranteed to support any other operations.

// Do not add, remove, or change any of the class's member variables.
// The num_deleted counter is *optional*. You might find it helpful, but it
// is not required to pass the lab assignment.

// Do not change the Bucket type.

// SUBMISSION INSTRUCTIONS:
// Submit this file, by itself, in a .tar.gz.
// Other files will be ignored.

#include <cstdint>
#include <functional> // where std::hash lives
#include <vector>
#include <cassert> // useful for debugging!

// A bucket's status tells you whether it's empty, occupied, 
// or contains a deleted element.
enum class Status : uint8_t {
    Empty,
    Occupied,
    Deleted
};

template<typename K, typename V, typename Hasher = std::hash<K>>
class HashTable {
    // used by the autograder; do not change/remove.
    friend class Verifier;
public:
    // A bucket has a status, a key, and a value.
    struct Bucket {
        // Do not modify Bucket.
        Status status = Status::Empty;
        K key;
        V val;
    };

    HashTable() {
        // TODO: a default constructor (possibly empty).

        // You can use the following to avoid implementing rehash_and_grow().
        // However, you will only pass the AG test cases ending in _C.
        // To pass the rest, start with at most 20 buckets and implement rehash_and_grow().
        //    buckets.resize(10000);
        buckets.resize(20);
    }

    size_t size() const {
        return num_elements;
    }

    // returns a reference to the value in the bucket with the key, if it
    // already exists. Otherwise, insert it with a default value, and return
    // a reference to the resulting bucket.
    V& operator[](const K& key) {
        rehash_and_grow();
        size_t index = getBucket(key);
        size_t insertPos = 0;
        size_t count = 0;
        bool hasDelete = false;
        while (buckets[index].status != Status::Empty && count < buckets.size()) {
            auto& element = buckets[index];
            if (element.status == Status::Occupied && element.key == key) {
                return element.val;
            }
            if (!hasDelete && element.status == Status::Deleted) {
                hasDelete = true;
                insertPos = index;
            }
           index = (index + 1) % buckets.size();
           ++count;
        }
        if (hasDelete) {
            index = insertPos;
        }
        auto &element = buckets[index];
        element.key = key;
        element.val = V();
        element.status = Status::Occupied;
        ++num_elements;
        return element.val;
    }

    // insert returns whether inserted successfully
    // (if the key already exists in the table, do nothing and return false).
    bool insert(const K& key, const V& val) {
        rehash_and_grow();
        size_t index = getBucket(key);
        size_t insertPos = 0;
        size_t count = 0;
        bool hasDelete = false;
        while (buckets[index].status != Status::Empty && count < buckets.size()) {
            auto &element = buckets[index];
            if (element.status == Status::Occupied && element.key == key) {
                return false;
            }
            if (!hasDelete && element.status == Status::Deleted) {
                hasDelete = true;
                insertPos = index;
            }
            index = (index + 1) % buckets.size();
            ++count;
        }
        if (hasDelete) {
            index = insertPos;
        }
        auto & element = buckets[index];
        element.key = key;
        element.val = val;
        element.status = Status::Occupied;
        ++num_elements;
        return true;
    }
    // erase returns the number of items remove (0 or 1)
    size_t erase(const K& key) {
        size_t index = getBucket(key);
        size_t count = 0;
        while (buckets[index].status != Status::Empty && count < buckets.size()) {
            auto &element = buckets[index];
            if (element.status == Status::Occupied && element.key == key) {
                element.status = Status::Deleted;
                --num_elements;
                return 1;
            }
            index = (index + 1) %buckets.size();
            ++count;
        }
        return 0;
    }

private:
    size_t num_elements = 0;
    size_t num_deleted = 0; // OPTIONAL: you don't need to use num_deleted to pass
    std::vector<Bucket> buckets;

    void rehash_and_grow() {
        // You can avoid implementing rehash_and_grow() by calling
        //    buckets.resize(10000);
        // in the constructor. However, you will only pass the AG test cases ending in _C.
        // To pass the rest, start with at most 20 buckets and implement rehash_and_grow().
        double loadFactor = static_cast<double>(num_elements) / static_cast<double>(buckets.size());
        // double loadFactor = static_cast<double>(num_elements / buckets.size());
        if (loadFactor > 0.5) {
            std::vector<Bucket> newBuck;
            newBuck.resize(2 * buckets.size());
            swap(buckets, newBuck);
            num_elements = 0;
            for (const auto& ele : newBuck) {
                if (ele.status == Status::Occupied) {
                    insert(ele.key, ele.val);
                }     
            }
        }
    }

    // You can add methods here if you like.
    size_t getBucket(const K& key) {
        Hasher hasher;
        return hasher(key) % buckets.size();
    }
    // TODO
};

#endif // HASHTABLE_H