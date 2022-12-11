/*
 * bag.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ALGO_DS_LINKED_BAG_H_
#define ALGO_DS_LINKED_BAG_H_

#include "atto/core/core.hpp"
#include "ds/linked/singly-node.hpp"

namespace algo {

/** ---- Bag implementation ---------------------------------------------------
 * class Bag<Item>
 * @brief Bag is an abstract data structure containing an unordered
 * collection of items. An item can be inserted but not removed.
 * The purpose of a Bag data structure is simply to hold a collection of
 * items without any specific over which it can iterate.
 *
 * The original Bag implementation in Algorithms implements the Java
 * iterable interface. The current class uses a SinglyNodeIterator as the
 * underlying iterable object.
 *
 * API Bag<Item>:
 *      Bag()                      create an empty bag
 *      ~Bag()                     remove all items and destroy the bag
 *
 *      bool is_empty()             is the bag empty?
 *      size_t size()               number of items in the bag
 *      void clear()                remove all items from the bag
 *
 *      void add(Item item)        add an item to the bag
 * @see Algorithms, 4th Edition by Robert Sedgewick, Kevin Wayne.
 */
template<typename Item>
class Bag {
public:
    /* Bag iterators. */
    typedef SinglyNodeIterator<Item> iterator;
    typedef SinglyNodeConstIterator<Item> const_iterator;

    iterator begin() { return iterator(head_); }
    const_iterator begin() const { return const_iterator(head_); }

    iterator end() { return iterator(nullptr); }
    const_iterator end() const { return const_iterator(nullptr); }

    /* Bag public API member functions. */
    bool is_empty(void) const;
    size_t size(void) const;
    void clear(void);

    void add(const Item &item);

    /* Bag constructor/destructor. */
    explicit Bag() : head_(nullptr), count_(0) {}
    ~Bag() { clear(); }
    /* Copy constructor/assignment. */
    Bag(const Bag &other);
    Bag<Item> &operator=(const Bag<Item> &other);
    /* Move constructor/assignment. */
    Bag(Bag &&other);
    Bag<Item> &operator=(Bag<Item> &&other);

private:
    SinglyNode<Item> *head_;    /* head node pointer */
    size_t count_;              /* node counter */

    void initialize(const_iterator first, const_iterator last);
}; /* class Bag */


/** ---- Bag constructors and assign functions --------------------------------
 * Bag<Item>
 * @brief Copy the contents of the other in the same order.
 */
template<typename Item>
Bag<Item>::Bag(const Bag<Item> &other) : Bag<Item>::Bag()
{
    initialize(other.begin(), other.end());
}

/**
 * Bag<Item>
 * @brief Copy the nodes of the other in the same order.
 * Ensure is empty before copying all the nodes.
 */
template<typename Item>
Bag<Item> &Bag<Item>::operator=(const Bag<Item> &other)
{
    if (this == &other) {
        return *this;
    }
    initialize(other.begin(), other.end());
    return *this;
}

/**
 * Bag<Item>
 * @brief Move the contents of the other without copying.
 * Afterwards, the other is a valid but unspecified object.
 */
template<typename Item>
Bag<Item>::Bag(Bag<Item> &&other) : Bag<Item>::Bag()
{
    head_  = std::move(other.head_);
    count_ = std::move(other.count_);
}

/**
 * Bag<Item>
 * @brief Clear and move the contents of the other without copying.
 * Afterwards, the other is a valid but unspecified object.
 */
template<typename Item>
Bag<Item> &Bag<Item>::operator=(Bag<Item> &&other)
{
    if (this == &other) {
        return *this;
    }

    clear();
    head_  = std::move(other.head_);
    count_ = std::move(other.count_);

    return *this;
}

/** ---- Bag private member functions -----------------------------------------
 * Bag::initialize
 * @brief Initialise the bag with the items in the range from first to last.
 * Called by the copy/assign constructor.
 */
template<typename Item>
core_inline
void Bag<Item>::initialize(const_iterator first, const_iterator last)
{
    clear();
    try {
        SinglyNode<Item> *to = head_;
        for (const_iterator it = first; it != last; ++it) {
            SinglyNode<Item> *node = atto::core::align_alloc<SinglyNode<Item>>(*it);
            if (to) {
                to->next_ = node;
            }
            to = node;
            ++count_;
        }
    } catch (std::exception& e) {
        clear();
        core_throw(e.what());
    }
}

/** ---- Bag API functions ----------------------------------------------------
 * Bag<Item>::is_empty
 * @brief Return true if the head doesn't point to a node.
 */
template<typename Item>
core_inline
bool Bag<Item>::is_empty(void) const
{
    return (head_ == nullptr);
}

/**
 * Bag<Item>::size
 * @brief Return the number of nodes in the bag.
 */
template<typename Item>
core_inline
size_t Bag<Item>::size(void) const
{
    return count_;
}

/**
 * Bag<Item>::clear
 * @brief Delete all nodes and reset head and count.
 */
template<typename Item>
core_inline
void Bag<Item>::clear(void)
{
    while (!is_empty()) {
        SinglyNode<Item> *node = head_->next_;
        atto::core::align_free<SinglyNode<Item>>(head_);
        head_ = node;
    }
    count_ = 0;
}

/** ---- Bag API mutator functions --------------------------------------------
 * Bag<Item>::add
 * @brief Create a new node at the head of the bag.
 */
template<typename Item>
core_inline
void Bag<Item>::add(const Item &item)
{
    SinglyNode<Item> *node = nullptr;
    try {
        node = atto::core::align_alloc<SinglyNode<Item>>(item);
    } catch (std::exception& e) {
        core_throw(e.what());
    }
    node->next_ = head_;
    head_ = node;

    ++count_;
}

} /* namespace algo */

#endif /* ALGO_DS_LINKED_BAG_H_ */
