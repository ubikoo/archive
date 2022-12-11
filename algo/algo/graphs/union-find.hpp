/*
 * union-find.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ALGO_GRAPHS_UNION_FIND_H_
#define ALGO_GRAPHS_UNION_FIND_H_

#include "atto/core/core.hpp"

namespace algo {

/** ---- UnionFind implementation ---------------------------------------------
 *
 * class UnionFind
 * @brief UnionFind implements a disjoint set data structure.
 * It supports union and find operations on the sets, together with a
 * count operation that returns the total number of set components.
 * This implementation uses the weighted-quick-union by size with path
 * compression during the find operation.
 *
 * UnionFind solves a dynamic connectivity problem. The input is taken
 * as a sequence of pairs of integers, where each integer represents an
 * object of some type.
 * The pair p and q are to be interpreted as meaning p "is connected to" q.
 * The term "is connected to" is assumed to be an equivalence relation:
 *
 *  symmetric:  if p is connected to q, then q is connected to p.
 *  transitive: if p is connected to q and q is connected to r,
 *              then p is connected to r.
 *  reflexive:  p is connected to p.
 *
 * The underlying data structure is a site indexed array which maintains
 * the invariant that p and q are connected if and only if the root parent
 * of p is equal to the root parent of q.
 * The find operation returns the canonical element of the set containing
 * a given element.
 * The join operation merges the set containing element p with the set
 * containing element q. Which root parent becomes child is determined
 * by the rank(or size) of the sets.
 *
 * API UnionFind:
 *      UnionFind()            create an empty disjoint-set data structure.
 *      UnionFind(n);          create a disjoint-set with n elements.
 *      ~UnionFind()           destroy all sets in the data structure.
 *
 *      size_t capacity()      return the number of keys.
 *      size_t count()         return the number of components.
 *      size_t size(p)         return the size of the tree containing p
 *      bool contains(p)       return true if p exists in the ensemble
 *      size_t find(p);        find the canonical component of element p
 *      void join(p, q)        merge set containing p with set containing q
 *      std::map<size_t, std::vector<size_t>> sets()
 *                              return a key-value map of all the components,
 *                              where key is the component parent index and
 *                              value is a vector of all the keys contained
 *                              in the component.
 *
 * @see Algorithms, 4th Edition by Robert Sedgewick, Kevin Wayne.
 */
class UnionFind {
public:
    /*
     * UnionFind public API member functions.
     */
    size_t capacity(void) const { return capacity_; }
    size_t count(void) const { return count_; }
    size_t size(size_t p) { return size_[find(p)]; }
    bool contains(size_t p) const;

    size_t find(size_t p);
    void join(size_t p, size_t q);

    std::map<size_t, std::vector<size_t>> sets(void);

    /*
     * UnionFind constructor/destructor.
     */
    UnionFind(size_t capacity)
        : parent_(capacity)
        , size_(capacity)
        , count_(capacity)
        , capacity_(capacity) {
        for (size_t i = 0; i < capacity; ++i) {
           parent_[i] = i;      /* each {key} is initially its parent set */
           size_[i] = 1;        /* with a single element */
        }
    }
    ~UnionFind() = default;
    /* Copy constructor/assignment. */
    UnionFind(const UnionFind &other)
        : parent_(other.parent_)
        , size_(other.size_)
        , count_(other.count_)
        , capacity_(other.capacity_) {}
    UnionFind &operator=(const UnionFind &other) {
        if (this == &other)
            return *this;
        parent_ = other.parent_;
        size_ = other.size_;
        count_ = other.count_;
        capacity_ = other.capacity_;
        return *this;
    }
    /* Move constructor/assignment. */
    UnionFind(UnionFind &&other)
        : parent_(std::move(other.parent_))
        , size_(std::move(other.size_))
        , count_(std::move(other.count_))
        , capacity_(std::move(other.capacity_)) {}
    UnionFind &operator=(UnionFind &&other) {
        if (this == &other)
            return *this;
        parent_ = std::move(other.parent_);
        size_ = std::move(other.size_);
        count_ = std::move(other.count_);
        capacity_ = std::move(other.capacity_);
        return *this;
    }

private:
    /*
     * UnionFind member variables.
     */
    std::vector<size_t> parent_;        /* parent index of each set */
    std::vector<size_t> size_;          /* component size of each set */
    size_t count_;                      /* number of disjoint sets */
    size_t capacity_;                   /* total number of keys */
}; /* class UnionFind */

/**
 * UnionFind::contains
 * @brief Return true if there is a set in the ensemble
 * containing the key and return false otherwise.
 */
core_inline
bool UnionFind::contains(size_t p) const
{
    return (p >= 0 && p < capacity_);
}

/**
 * UnionFind::find
 * @brief Find the component to which p-key belongs to.
 * The find operation uses path compression - every element between
 * the the query value p and the root is set to to point to the root.
 *
 * Path compression can be implemented using two passes - one to find
 * the root and the second to compress the path towards the root id.
 */
core_inline
size_t UnionFind::find(size_t p)
{
    core_assert(contains(p), "non existent key");

    /*
     * Find the root element of p
     */
    size_t root = p;
    while (root != parent_[root]) {
        root = parent_[root];
    }

    /*
     * Compress the path from p towards its root.
     */
    size_t next = p;
    while (next != parent_[next]) {
        size_t curr = next;
        next = parent_[next];
        parent_[curr] = root;
    }

    return root;
}

/**
 * UnionFind::join
 * @brief Join the component of p-value and the component of q-value.
 * If these are already connected, there is nothing to do.
 * Otherwise, merge the canonical components of p and q.
 */
core_inline
void UnionFind::join(size_t p, size_t q)
{
    /* Get the components of p and q. */
    size_t root_p = find(p);
    size_t root_q = find(q);

    /* Both p and q belong to the same component - nothing to do. */
    if (root_p == root_q) {
        return;
    }

    /*
     * Join the smaller set into the larger set to minimize tree depth after
     * the merge operation.
     * If the p-set is smaller than q-set, merge p-set into q-set and increment
     * q-size with p-size.
     * Otherwise, merge q-set into p-set and increment p-size.
     */
    if (size_[root_p] < size_[root_q]) {
        parent_[root_p] = root_q;
        size_[root_q] += size_[root_p];
    } else {
        parent_[root_q] = root_p;
        size_[root_p] += size_[root_q];
    }

    count_--;
}

/**
 * UnionFind::sets
 * @brief Return a key-value map of the components in the ensemble.
 * The keys represent the parent set index of each component.
 * The values are vectors containing the keys in each component.
 */
core_inline
std::map<size_t, std::vector<size_t>> UnionFind::sets(void)
{
    std::map<size_t, std::vector<size_t>> components;
    for (size_t p = 0; p < capacity_; ++p) {
        components[find(p)].push_back(p);
    }
    return components;
}


/** ---- IndexUnionFind implementation ----------------------------------------
 *
 * class IndexUnionFind<Key>
 * @brief IndexUnionFind maintains a disjoint-set data structure.
 * It supports union and find operations on the sets, together with a count
 * operation that returns the total number of component sets.
 * This implementation uses the weighted-quick-union by size with path
 * compression during the find operation.
 *
 * IndexUnionFind solves a dynamic connectivity problem. The input is taken as
 * a sequence of pairs of integers, where each integer represents an object of
 * some type.
 * The pair p and q are to be interpreted as meaning p "is connected to" q.
 * The term "is connected to" is assumed to be an equivalence relation:
 *
 *  symmetric:  if p is connected to q, then q is connected to p.
 *  transitive: if p is connected to q and q is connected to r,
 *              then p is connected to r.
 *  reflexive:  p is connected to p.
 *
 * The underlying data structure is a site indexed array which maintains the
 * invariant that p and q are connected if and only if the root parent of p
 * is equal to the root parent of q.
 * The find operation returns the canonical element of the set containing a
 * given element.
 * The join operation merges the set containing element p with the set
 * containing element q. Which root parent becomes child is determined
 * by the rank(or size) of the sets.
 *
 * API IndexUnionFind<Key>:
 *      IndexUnionFind()       create an empty disjoint-set data structure
 *      IndexUnionFind(n);     create a disjoint-set with n elements.
 *      ~IndexUnionFind()      destroy all sets in the data structure
 *
 *      size_t capacity()      return the number of keys.
 *      size_t count()         return the number of components.
 *      size_t size(key)       return the size of the tree containing key
 *      bool contains(key)     return true if key exists in the ensemble
 *
 *      size_t insert(key)     create a new set in the ensemble with key
 *      size_t find(key)       find the canonical component of key
 *      void join(p, q)        merge set containing p with set containing q
 *
 *      void clear(void)       clear all the sets in the ensemble
 *      void merge(other)      merge all the sets from another ensemble
 *      std::map<size_t, std::vector<size_t>> sets()
 *                              return a key-value map of all the components,
 *                              where key is the component parent index and
 *                              value is a vector of all the keys contained
 *                              in the component.
 *
 * @see Algorithms, 4th Edition by Robert Sedgewick, Kevin Wayne.
 */
template<typename Key>
class IndexUnionFind {
public:
    /*
     * IndexUnionFind public API member functions.
     */
    size_t capacity(void) const { return capacity_; }
    size_t count(void) const { return count_; }
    size_t size(const Key &key) { return size_[find(key)]; }
    bool contains(const Key &key) const;

    size_t insert(const Key &key);
    size_t find(const Key &key);
    void join(const Key &key_p, const Key &key_q);

    void clear(void);
    void merge(const IndexUnionFind &other);
    std::map<size_t, std::vector<size_t>> sets(void);

    /*
     * IndexUnionFind constructor/destructor.
     */
    IndexUnionFind() : count_(0), capacity_(0) {}
    ~IndexUnionFind() = default;
    /* Copy constructor/assignment. */
    IndexUnionFind(const IndexUnionFind &other)
        : index_(other.index_)
        , parent_(other.parent_)
        , size_(other.size_)
        , count_(other.count_)
        , capacity_(other.capacity_) {}
    IndexUnionFind &operator=(const IndexUnionFind &other) {
        if (this == &other)
            return *this;
        index_ = other.index_;
        parent_ = other.parent_;
        size_ = other.size_;
        count_ = other.count_;
        capacity_ = other.capacity_;
        return *this;
    }
    /* Move constructor/assignment. */
    IndexUnionFind(IndexUnionFind &&other)
        : index_(std::move(other.index_))
        , parent_(std::move(other.parent_))
        , size_(std::move(other.size_))
        , count_(std::move(other.count_))
        , capacity_(std::move(other.capacity_)) {}
    IndexUnionFind &operator=(IndexUnionFind &&other) {
        if (this == &other)
            return *this;
        index_ = std::move(other.index_);
        parent_ = std::move(other.parent_);
        size_ = std::move(other.size_);
        count_ = std::move(other.count_);
        capacity_ = std::move(other.capacity_);
        return *this;
    }

private:
    /*
     * IndexUnionFind member variables.
     */
    std::map<Key, size_t> index_;       /* key-set index map */
    std::vector<size_t> parent_;        /* parent index of each set */
    std::vector<size_t> size_;          /* component size of each set */
    size_t count_;                      /* number of disjoint sets */
    size_t capacity_;                   /* total number of keys */
}; /* class IndexUnionFind */


/**
 * IndexUnionFind<Key>::contains
 * @brief Return true if there is a set in the ensemble
 * containing the key and return false otherwise.
 */
template<typename Key>
core_inline
bool IndexUnionFind<Key>::contains(const Key &key) const
{
    return (index_.find(key) != index_.end());
}

/**
 * IndexUnionFind<Key>::insert
 * @brief Create a new set {key} containing the key.
 * The key object must not appear in any other set in the ensemble.
 * The parent of the new set is obviously the key itself.
 */
template<typename Key>
core_inline
size_t IndexUnionFind<Key>::insert(const Key &key)
{
    core_assert(!contains(key), "duplicate key in the ensemble");

    size_t id = index_.size();  /* unique index for the new set */
    index_[key] = id;           /* create a new set {key} with index id */
    parent_.push_back(id);      /* each {key} is initially its parent set */
    size_.push_back(1);         /* with a single element */
    count_++;                   /* increment number of components */
    capacity_++;                /* increment number of keys */

    return index_[key];
}

/**
 * IndexUnionFind<Key>::find
 * @brief Find the component to which p-key belongs to.
 * The find operation uses path compression - every element between
 * the the query key p and the root is set to to point to the root.
 *
 * Path compression can be implemented using two passes - one to find
 * the root and the second to compress the path towards the root id.
 */
template<typename Key>
core_inline
size_t IndexUnionFind<Key>::find(const Key &key)
{
    //core_assert(contains(key), "non existent key");

    /*
     * Find the root component of key.
     */
    size_t root = index_[key];
    while (root != parent_[root]) {
        root = parent_[root];
    }

    /*
     * Compress the path from key towards its root.
     */
    size_t next = index_[key];
    while (next != parent_[next]) {
        size_t curr = next;
        next = parent_[next];
        parent_[curr] = root;
    }

    return root;
}

/**
 * IndexUnionFind<Key>::join
 * @brief Join the component of p-key and the component of q-key.
 * If these are already connected, there is nothing to do.
 * Otherwise, merge the canonical components of p and q.
 */
template<typename Key>
core_inline
void IndexUnionFind<Key>::join(const Key &key_p, const Key &key_q)
{
    /* Get the components of p and q. */
    size_t root_p = find(key_p);
    size_t root_q = find(key_q);

    /* Both p and q belong to the same component - nothing to do. */
    if (root_p == root_q) {
        return;
    }

    /*
     * Join the smaller set into the larger set to minimize
     * the tree depth after the merge operation.
     * If the p-set is smaller than q-set, merge p-set into q-set
     * and increment q-size with p-size.
     * Otherwise, merge q-set into p-set and increment p-size.
     */
    if (size_[root_p] < size_[root_q]) {
        parent_[root_p] = root_q;
        size_[root_q] += size_[root_p];
    } else {
        parent_[root_q] = root_p;
        size_[root_p] += size_[root_q];
    }

    count_--;
}

/**
 * IndexUnionFind<Key>::clear
 * @brief Clear all the union find data structures.
 */
template<typename Key>
core_inline
void IndexUnionFind<Key>::clear(void)
{
    index_.clear();     /* clear the set of key ids */
    parent_.clear();    /* clear the key parent ids */
    size_.clear();      /* clear the key set size */
    count_ = 0;         /* reset the component count */
}

/**
 * IndexUnionFind<Key>::merge
 * @brief Merge the sets of the rhs ensemble into the current one.
 */
template<typename Key>
core_inline
void IndexUnionFind<Key>::merge(const IndexUnionFind &rhs)
{
    size_t offset = index_.size();          /* offset for each new key id */

    for (const auto &it : rhs.index_) {
        const Key &key = it.first;          /* rhs key reference */
        const size_t &id = it.second;       /* rhs key id */

        const size_t parent = rhs.parent_[id];
        const size_t size = rhs.size_[id];

        core_assert(!contains(key), "duplicate key in the ensemble");

        index_[key] = id + offset;          /* set {key} with offset id */
        parent_.push_back(parent + offset); /* parent set with offset id */
        size_.push_back(size);              /* with size equal to rhs size */
    }

    count_ += rhs.count_;           /* increment the number of components */
    capacity_ += rhs.capacity_;     /* increment the number of keys */
}

/**
 * IndexUnionFind<Key>::sets
 * @brief Return a key-value map of the components in the ensemble.
 * The keys represent the parent set index of each component.
 * The values are vectors containing the keys in each component.
 */
template<typename Key>
core_inline
std::map<size_t, std::vector<size_t>> IndexUnionFind<Key>::sets(void)
{
    std::map<size_t, std::vector<Key>> components;
    for (auto it : index_) {
        const Key &key = it.first;      /* get key reference */
        components[find(key)].push_back(key);
    }
    return components;
}

} /* namespace algo */

#endif /* ALGO_GRAPHS_UNION_FIND_H_ */
