/*
 * singly-node.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ALGO_DS_LINKED_SINGLY_NODE_H_
#define ALGO_DS_LINKED_SINGLY_NODE_H_

#include "atto/core/core.hpp"

namespace algo {

/** ---- SinglyNode -----------------------------------------------------------
 * class SinglyNode<Item>
 * @brief SinglyNode is a recursive data type containing a data item
 * and a reference to another node of the same type.
 * If the reference is null, there is no referenced node.
 *
 * It is an abstraction of an associative data structure used to by
 * more complex data structures - bags, stacks and queues.
 *
 * @note
 * Every abstract data structure that is a friend of SinglyNode class has
 * access to its members, functions and constructors. These are all
 * marked as private thereby blocking everything except these.
 *
 * @see Algorithms 4th Edition by Robert Sedgewick, Kevin Wayne.
 *      https://en.wikipedia.org/wiki/Linked_list
 */
template<typename Item> class Bag;
template<typename Item> class Stack;
template<typename Item> class Queue;
template<typename Item> class SinglyNodeIterator;
template<typename Item> class SinglyNodeConstIterator;

template<typename Item>
class SinglyNode {
public:
    /* Grant access to Bag, Stack, Queue and SinglyNodeIterator classes */
    friend class Bag<Item>;
    friend class Stack<Item>;
    friend class Queue<Item>;
    friend class SinglyNodeIterator<Item>;
    friend class SinglyNodeConstIterator<Item>;

    /* Grant access to core align_alloc and align_free */
    friend SinglyNode<Item> *atto::core::align_alloc<SinglyNode<Item>>();
    friend SinglyNode<Item> *atto::core::align_alloc<SinglyNode<Item>>(const Item &);
    friend void atto::core::align_free<SinglyNode<Item>>(SinglyNode<Item> *);

private:
    /* SinglyNode members */
    SinglyNode<Item> *next_;
    Item item_;

    /* SinglyNode constructor/destructor. */
    explicit SinglyNode()
        : next_(nullptr) {}
    explicit SinglyNode(const Item &item)
        : next_(nullptr)
        , item_(item) {}
    ~SinglyNode() {}
    /* Copy constructor/assignment. */
    SinglyNode(const SinglyNode &other)
        : next_(other.next_)
        , item_(other.item_) {}
    SinglyNode &operator=(const SinglyNode &other) {
        if (this == &other)
            return *this;
        next_ = other.next_;
        item_ = other.item_;
        return *this;
    }
    /* Move constructor/assignment. */
    SinglyNode(SinglyNode &&other)
        : next_(std::move(other.next_))
        , item_(std::move(other.item_)) {}
    SinglyNode &operator=(SinglyNode &&other) {
        if (this == &other)
            return *this;
        next_ = std::move(other.next_);
        item_ = std::move(other.item_);
        return *this;
    }
}; /* class SinglyNode */

/** ---- SinglyNodeIterator ---------------------------------------------------
 * SinglyNodeIterator<Item>
 * @brief SinglyNodeIterator is a forward iterator, accessing
 * a list of nodes in the direction from head to tail.
 * It needs to implement - at least - the following:
 *
 *  X a;                // default-constructor and destructor.
 *  X b(a);             // copy-constructor.
 *  X b = a;            // copy-assign.
 *
 *  *a, a->m;           // dereference operators(const).
 *  ++a, a++, *a++;     // increment operators(dereferenceable).
 *  a == b, a != b;     // comparison operators(const).
 *  swap(a,b);          // swap operator.
 *
 * The implementation needs to define the following types describing
 * the iterator properties:
 *
 *  typedef typename Iterator::value_type            value_type;
 *  typedef typename Iterator::pointer               pointer;
 *  typedef typename Iterator::reference             reference;
 *  typedef typename Iterator::difference_type       difference_type;
 *  typedef typename Iterator::iterator_category     iterator_category;
 *
 * Standard Library containers provide two types of iterators,
 *  iterator type - pointing to mutable data, and
 *  const_iterator type - pointing to immutable data.
 *
 * It is possible to support both by providing a conversion operator:
 *  operator Iterator<const Item>() const {
 *      return Iterator<const Item>(itr);
 *  }
 *
 * Or by defining explicitly a SinglyNodeConstIterator. The later is used here.
 *
 * @see
 * http://www.cplusplus.com/reference/iterator
 * https://en.cppreference.com/w/cpp/iterator
 * https://www.justsoftwaresolutions.co.uk/cplusplus/generating_sequences.html
 * https://codereview.stackexchange.com/questions/74609
 */
template<typename Item>
class SinglyNodeIterator {
public:
    /* Iterator member types. */
    typedef SinglyNodeIterator<Item>    self;

    typedef Item                        value_type;
    typedef Item*                       pointer;
    typedef Item&                       reference;
    typedef ptrdiff_t                   difference_type;
    typedef std::forward_iterator_tag   iterator_category;

    /* Dereference operators(const) */
    reference operator*() const {
        core_assert(node_ != nullptr, "invalid iterator dereference");
        return node_->item_;
    }

    pointer operator->() const {
        core_assert(node_ != nullptr, "invalid iterator dereference");
        return &(operator*());
    }

    /* Increment operators */
    self operator++() {
        core_assert(node_ != nullptr, "invalid iterator increment");
        node_ = node_->next_;
        return *this;
    }

    self operator++(int) {
        core_assert(node_ != nullptr, "invalid iterator increment");
        SinglyNodeIterator<Item> tmp(*this);
        node_ = node_->next_;
        return tmp;
    }

    /* Comparison operators(const) */
    bool operator==(const SinglyNodeIterator<Item> &other) const {
        return (node_ == other.node_);
    }

    bool operator!=(const SinglyNodeIterator<Item> &other) const {
        return !operator==(other);
    }

    /* Swap operator */
    void swap(SinglyNodeIterator &other) {
        std::swap(node_, other.node_);
    }

    /* SinglyNodeIterator constructor/destructor. */
    SinglyNodeIterator() : node_() {}
    explicit SinglyNodeIterator(SinglyNode<Item> *node) : node_(node) {}
    ~SinglyNodeIterator() = default;

private:
    SinglyNode<Item> *node_;
}; /* class SinglyNodeIterator */


/** ---- SinglyNodeConstIterator ----------------------------------------------
 * SinglyNodeConstIterator<Item>
 * @brief SinglyNodeConstIterator is a constant forward iterator.
 * It is the constant counterpart of SinglyNodeIterator.
 */
template<typename Item>
class SinglyNodeConstIterator {
public:
    /* Iterator member types. */
    typedef SinglyNodeConstIterator<Item>   self;

    typedef Item                            value_type;
    typedef const Item*                     pointer;
    typedef const Item&                     reference;
    typedef ptrdiff_t                       difference_type;
    typedef std::forward_iterator_tag       iterator_category;

    /* Constructor/destructor */
    SinglyNodeConstIterator() : node_() {}
    explicit SinglyNodeConstIterator(SinglyNode<Item> *node) : node_(node) {}
    ~SinglyNodeConstIterator() = default;

    /* Dereference operators(const) */
    reference operator*() const {
        core_assert(node_ != nullptr, "invalid const_iterator dereference");
        return node_->item_;
    }

    pointer operator->() const {
        core_assert(node_ != nullptr, "invalid const_iterator dereference");
        return &(operator*());
    }

    /* Increment operators */
    self operator++() {
        core_assert(node_ != nullptr, "invalid const_iterator increment");
        node_ = node_->next_;
        return *this;
    }

    self operator++(int) {
        core_assert(node_ != nullptr, "invalid const_iterator increment");
        SinglyNodeConstIterator<Item> tmp(*this);
        node_ = node_->next_;
        return tmp;
    }

    /* Comparison operators(const) */
    bool operator==(const SinglyNodeConstIterator<Item> &other) const {
        return (node_ == other.node_);
    }

    bool operator!=(const SinglyNodeConstIterator<Item> &other) const {
        return !operator==(other);
    }

    /* Swap operator */
    void swap(SinglyNodeConstIterator &other) {
        std::swap(node_, other.node_);
    }

private:
    const SinglyNode<Item> *node_;
}; /* class SinglyNodeConstIterator */

} /* namespace algo */

#endif /* ALGO_DS_LINKED_SINGLY_NODE_H_ */
