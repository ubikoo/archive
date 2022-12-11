/*
 * queue.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ALGO_DS_LINKED_QUEUE_H_
#define ALGO_DS_LINKED_QUEUE_H_

#include "atto/core/core.hpp"
#include "ds/linked/singly-node.hpp"

namespace algo {

/** ---- Queue implementation -------------------------------------------------
 * class Queue<Item>
 * @brief Queue is an abstract data structure containing a collection of items
 * in last-in-first-out LIFO order.
 *
 * The original Queue implementation in Algorithms implements the Java
 * iterable interface. The current class uses a NodeIterator as the
 * underlying iterable object.
 *
 * API Queue<Item>:
 *      Queue()                    create an empty queue
 *      ~Queue()                   remove all items and destroy the queue
 *
 *      bool is_empty()             is the queue empty?
 *      size_t size()               number of items in the queue
 *      void clear()                remove all items from the queue
 *
 *      void enqueue(Item item)    add an item to the queue
 *      void dequeue()             remove the last item from the queue
 *
 *      Item &front()               access the front item
 *      const Item &front()         const access the front item
 *      Item &back()                access the back item
 *      const Item &back()          const access the back item
 *
 * @see Algorithms, 4th Edition by Robert Sedgewick, Kevin Wayne.
 */
template<typename Item>
class Queue {
public:
    /* Queue iterators. */
    typedef SinglyNodeIterator<Item> iterator;
    typedef SinglyNodeConstIterator<Item> const_iterator;

    iterator begin() { return iterator(head_); }
    const_iterator begin() const { return const_iterator(head_); }

    iterator end() { return iterator(nullptr); }
    const_iterator end() const { return const_iterator(nullptr); }

    /* Queue public API member functions. */
    bool is_empty(void) const;
    size_t size(void) const;
    void clear(void);

    void enqueue(const Item &item);
    void dequeue(void);

    Item &front(void);
    const Item &front(void) const;

    Item &back(void);
    const Item &back(void) const;

    /* Queue constructor/destructor. */
    explicit Queue() : head_(nullptr), tail_(nullptr), count_(0) {}
    ~Queue() { clear(); }
    /* Copy constructor/assignment. */
    Queue(const Queue &other);
    Queue<Item> &operator=(const Queue<Item> &other);
    /* Move constructor/assignment. */
    Queue(Queue &&other);
    Queue<Item> &operator=(Queue<Item> &&other);

private:
    SinglyNode<Item> *head_;    /* head node pointer */
    SinglyNode<Item> *tail_;    /* tail node pointer */
    size_t count_;              /* node counter */

    void initialize(const_iterator first, const_iterator last);
}; /* class Queue */

/** ---- Queue constructors and assign functions ------------------------------
 * Queue<Item>
 * @brief Copy the contents of the other queue in the same order.
 */
template<typename Item>
Queue<Item>::Queue(const Queue<Item> &other) : Queue<Item>::Queue()
{
    initialize(other.begin(), other.end());
}

/**
 * Queue<Item>
 * @brief Copy the contents of the other queue in the same order.
 * Ensure queue is empty before copying all nodes.
 */
template<typename Item>
Queue<Item> &Queue<Item>::operator=(const Queue<Item> &other)
{
    if (this == &other) {
        return *this;
    }
    initialize(other.begin(), other.end());
    return *this;
}

/**
 * Queue<Item>
 * @brief Move the contents of the other without copying.
 * Afterwards, the other is a valid but unspecified object..
 */
template<typename Item>
Queue<Item>::Queue(Queue<Item> &&other) : Queue<Item>::Queue()
{
    head_  = std::move(other.head_);
    tail_  = std::move(other.head_);
    count_ = std::move(other.count_);
}

/**
 * Queue<Item>
 * @brief Clear and move the contents of the other without copying.
 * Afterwards, the other is a valid but unspecified object.
 */
template<typename Item>
Queue<Item> &Queue<Item>::operator=(Queue<Item> &&other)
{
    if (this == &other) {
        return *this;
    }

    clear();
    head_  = std::move(other.head_);
    tail_  = std::move(other.head_);
    count_ = std::move(other.count_);

    return *this;
}

/** ---- Queue private member functions ---------------------------------------
 * Queue::initialize
 * @brief Initialise the queue with the items in the range from first to last.
 * Called by the copy/assign constructor.
 */
template<typename Item>
core_inline
void Queue<Item>::initialize(const_iterator first, const_iterator last)
{
    clear();
    for (const_iterator it = first; it != last; ++it) {
        enqueue(*it);
    }
}

/** ---- Queue API functions --------------------------------------------------
 * Queue<Item>::is_empty
 * @brief Return true if the head doesn't point to a node.
 */
template<typename Item>
core_inline
bool Queue<Item>::is_empty(void) const
{
    return (head_ == nullptr);
}

/**
 * Queue<Item>::size
 * @brief Return the number of nodes in the queue.
 */
template<typename Item>
core_inline
size_t Queue<Item>::size(void) const
{
    return count_;
}

/**
 * Queue<Item>::clear
 * @brief Delete all nodes and reset head and count.
 */
template<typename Item>
core_inline
void Queue<Item>::clear(void)
{
    while (!is_empty()) {
        dequeue();
    }
}

/** ---- Queue API mutator functions ------------------------------------------
 * Queue<Item>::enqueue
 * @brief Create a new node at the head of the queue.
 */
template<typename Item>
core_inline
void Queue<Item>::enqueue(const Item &item)
{
    SinglyNode<Item> *node = nullptr;
    try {
        node = atto::core::align_alloc<SinglyNode<Item>>(item);
    } catch (std::exception& e) {
        core_throw(e.what());
    }

    /*
     * If queue is empty, update both head and tail.
     * Otherwise, link the back node to the new one.
     */
    if (is_empty()) {
        head_ = node;
    } else {
        tail_->next_ = node;
    }
    tail_ = node;

    ++count_;
}

/**
 * Queue<Item>::dequeue
 * @brief Delete a node from the front of the queue.
 */
template<typename Item>
core_inline
void Queue<Item>::dequeue(void)
{
    core_assert(!is_empty(), "empty queue, out of range error");

    SinglyNode<Item> *node = head_->next_;
    atto::core::align_free<SinglyNode<Item>>(head_);
    head_ = node;

    if (is_empty()) {
        tail_ = nullptr;    /* if queue is empty, reset the tail too. */

    }
    --count_;
}

/** ---- Queue API accessor functions -----------------------------------------
 * Queue<Item>::front
 */
template<typename Item>
core_inline
Item &Queue<Item>::front(void)
{
    core_assert(!is_empty(), "empty queue, out of range error");
    return head_->item_;
}

/**
 * Queue<Item>::front
 */
template<typename Item>
core_inline
const Item &Queue<Item>::front(void) const
{
    core_assert(!is_empty(), "empty queue, out of range error");
    return head_->item_;
}

/**
 * Queue<Item>::back
 */
template<typename Item>
core_inline
Item &Queue<Item>::back(void)
{
    core_assert(!is_empty(), "empty queue, out of range error");
    return tail_->item_;
}

/**
 * Queue<Item>::back
 */
template<typename Item>
core_inline
const Item &Queue<Item>::back(void) const
{
    core_assert(!is_empty(), "empty queue, out of range error");
    return tail_->item_;
}

} /* namespace algo */

#endif /* ALGO_DS_LINKED_QUEUE_H_ */
