/*
 * test-priority-queue.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_ALGO_SORT_PRIORITY_ARGQUEUE_H_
#define TEST_ALGO_SORT_PRIORITY_ARGQUEUE_H_

#include "algo/sort/priority-queue.hpp"

/** ---- Priority node struct --------------------------------------------------
 *
 * struct PriorityNode
 *
 * Test the priority queue using a node structure.
 */
struct PriorityNode {
    /* Member variables/accessors */
    size_t v_ = 0;
    void v(size_t val) { v_ = val; }
    const size_t v(void) const { return v_; }

    /* Constructor/destructor */
    PriorityNode() = default;
    PriorityNode(size_t v) : v_(v) {}
    ~PriorityNode() = default;

    /* Copy constructor/assignment */
    PriorityNode(const PriorityNode &other) : v_(other.v_) {}
    PriorityNode &operator=(const PriorityNode &other) {
        if (this == &other)
            return *this;
        v_ = other.v_;
        return *this;
    }

    /* Move constructor/assignment */
    PriorityNode(PriorityNode &&other) : v_(std::move(other.v_)) {}
    PriorityNode &operator=(PriorityNode &&other) {
        if (this == &other)
            return *this;
        v_ = std::move(other.v_);
        return *this;
    }
};


/** ---- IndexPriorityQueue print function -------------------------------------
 *
 * test_priority_queue
 */
template<typename Key, typename Compare>
void print_queue(const std::string &name,
                  const algo::PriorityQueue<Key, Compare> &queue)
{
    algo::PriorityQueue<Key, Compare> copy(queue);
    std::cout << name << std::endl;
    while (!copy.is_empty()) {
        std::cout << copy.top() << " ";
        copy.pop();
    }
    std::cout << std::endl;
}


/** ---- IndexPriorityQueue print function -------------------------------------
 *
 * test_priority_queue
 */
template<typename Key, typename Compare>
void print_queue(const std::string &name,
                  const algo::IndexPriorityQueue<Key, Compare> &queue)
{
    algo::IndexPriorityQueue<Key, Compare> copy(queue);
    std::cout << name << std::endl;
    while (!copy.is_empty()) {
        std::cout << copy.top() << " ";
        copy.pop();
    }
    std::cout << std::endl;
}

#endif /* TEST_ALGO_SORT_PRIORITY_ARGQUEUE_H_ */
