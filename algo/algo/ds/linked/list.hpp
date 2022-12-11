/*
 * list.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ALGO_DS_LINKED_LIST_H_
#define ALGO_DS_LINKED_LIST_H_

#include "atto/core/core.hpp"
#include "ds/linked/doubly-node.hpp"

namespace algo {

/** ---- List implementation --------------------------------------------------
 * class List<Item>
 * @brief List represents a finite list of N(>=0) elements stored in
 * double linked order over which it can iterate.
 *
 * Each doubly node contains three fields - a link to the previous
 * node, a link to the next and a data item.
 * The class supports front (and back) insertion (and removal) of
 * items with constant O(1) complexity, and forward(and reverse)
 * iteration.
 *
 * The list has two pointers - head and tail - signalling the first
 * and last nodes in the list respectivelly.
 * If the list has only a single node, then the node is the first
 * and also the last - head and tail point to the same node.
 * If the list is empty, head and tail pointers are nullptr.
 *
 *  API List<Item>:
 *      List()                 create an empty list
 *      ~List()                remove all items and destroy the list
 *
 *      bool is_empty()         is the list empty?
 *      size_t size()           number of items in the list
 *      void clear()            remove all items from the list
 *
 *      void push_front(item)   insert an item at the front of the list
 *      void push_back(item)    insert an item at the back of the list
 *      void pop_front()        remove an item from the front of the list
 *      void pop_back()         remove an item from the back of the list
 *
 *      Item &front()           access the front item
 *      const Item &front()     const access the front item
 *      Item &back()            access the back item
 *      const Item &back()      const access the back item
 *
 * @see
 * Algorithms 4th Edition by Robert Sedgewick, Kevin Wayne.
 */
template<typename Item>
class List {
public:
    /* List iterators. */
    typedef DoublyNodeIterator<Item> iterator;
    typedef DoublyNodeConstIterator<Item> const_iterator;

    typedef DoublyNodeReverseIterator<Item> reverse_iterator;
    typedef DoublyNodeConstReverseIterator<Item> const_reverse_iterator;

    iterator begin() { return iterator(head_); }
    const_iterator begin() const { return const_iterator(head_); }

    iterator end() { return iterator(nullptr); }
    const_iterator end() const { return const_iterator(nullptr); }

    reverse_iterator rbegin() { return reverse_iterator(tail_); }
    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(tail_);
    }

    reverse_iterator rend() { return reverse_iterator(nullptr); }
    const_reverse_iterator rend() const {
        return const_reverse_iterator(nullptr);
    }

    /* List public API member functions. */
    bool is_empty(void) const;
    size_t size(void) const;
    void clear(void);

    void push_front(const Item &item);
    void push_back(const Item &item);

    void pop_front(void);
    void pop_back(void);

    Item &front(void);
    const Item &front(void) const;

    Item &back(void);
    const Item &back(void) const;

    /* List constructor/destructor. */
    explicit List() : head_(nullptr), tail_(nullptr), count_(0) {}
    ~List() { clear(); }
    /* Copy constructor/assignment. */
    List(const List &other);
    List<Item> &operator=(const List<Item> &other);
    /* Move constructor/assignment. */
    List(List &&other);
    List<Item> &operator=(List<Item> &&other);

private:
    DoublyNode<Item> *head_;        /* head node pointer */
    DoublyNode<Item> *tail_;        /* tail node pointer */
    size_t count_;                  /* node counter */

    void link(DoublyNode<Item> *left, DoublyNode<Item> *right);
    void unlink(DoublyNode<Item> *curr);
    void insert_before(DoublyNode<Item> *position, DoublyNode<Item> *node);
    void insert_after(DoublyNode<Item> *position, DoublyNode<Item> *node);
    void erase(DoublyNode<Item> *node);
}; /* class List */

/** ---- List constructors and assign functions -------------------------------
 * List<Item>
 * @brief Copy the contents of the other list in the same order.
 */
template<typename Item>
List<Item>::List(const List<Item> &other)
    : List<Item>::List()
{
    for (const auto it : other) {
        push_back(it);  /* insert at the back to preserve sequence order */
    }
}

/**
 * List<Item>
 * @brief Copy the contents of the other list in the same order.
 * Ensure list is empty before copying all nodes.
 */
template<typename Item>
List<Item> &List<Item>::operator=(const List<Item> &other)
{
    if (this == &other) {
        return *this;
    }

    clear();
    for (const auto &it : other) {
        push_back(it);      /* insert at the back to preserve sequence order */
    }

    return *this;
}

/**
 * List<Item>
 * @brief Move the contents of the other without copying.
 * Afterwards, the other is a valid but unspecified object.
 */
template<typename Item>
List<Item>::List(List<Item> &&other)
    : List<Item>::List()
{
    head_  = std::move(other.head_);
    tail_  = std::move(other.tail_);
    count_ = std::move(other.count_);
}

/**
 * List<Item>
 * @brief Clear and move the contents of the other without copying.
 * Afterwards, the other is a valid but unspecified object.
 */
template<typename Item>
List<Item> &List<Item>::operator=(List<Item> &&other)
{
    if (this == &other) {
        return *this;
    }

    clear();
    head_  = std::move(other.head_);
    tail_  = std::move(other.tail_);
    count_ = std::move(other.count_);

    return *this;
}

/** ---- List private member functions ----------------------------------------
 * List<Item>::link
 * @brief Link the left node to the right node.
 */
template<typename Item>
core_inline
void List<Item>::link(
    DoublyNode<Item> *left,
    DoublyNode<Item> *right)
{
    left->next_ = right;
    right->prev_ = left;
}

/**
 * List<Item>::unlink
 * @brief Unlink the node from the list.
 * Note that deleting the last node of the list (with null curr->next
 * and curr->prev pointers) sets both head and tail pointers to null,
 * thereby handling the removal from a one-element list.
 */
template<typename Item>
core_inline
void List<Item>::unlink(DoublyNode<Item> *curr)
{
    /*
     * If we have a previous node, link it to the next node link.
     * Otherwise current node is the front, relink the head instead.
     */
    if (curr->prev_) {
        curr->prev_->next_ = curr->next_;
    } else {
        head_ = curr->next_;
    }

    /*
     * If we have a next node, link it to the previous node link.
     * Otherwise current node is the back, relink the tail instead.
     */
    if (curr->next_) {
        curr->next_->prev_ = curr->prev_;
    } else {
        tail_ = curr->prev_;
    }
}

/**
 * List<Item>::insert_before
 * @brief Insert a node before a given position position.
 */
template<typename Item>
core_inline
void List<Item>::insert_before(
    DoublyNode<Item> *position,
    DoublyNode<Item> *node)
{
    if (position->prev_) {
       link(position->prev_, node);
    }
    link(node, position);
}

/**
 * List<Item>::insert_after
 * @brief Insert a node after a given position.
 */
template<typename Item>
core_inline
void List<Item>::insert_after(
    DoublyNode<Item> *position,
    DoublyNode<Item> *node)
{
    if (position->next_) {
       link(node, position->next_);
    }
    link(position, node);
}

/**
 * List<Item>::erase
 * @brief Unlink and delete a node from the list.
 */
template<typename Item>
core_inline
void List<Item>::erase(DoublyNode<Item> *node)
{
    unlink(node);
    atto::core::align_free<DoublyNode<Item>>(node);
}

/** ---- List API functions ---------------------------------------------------
 * List<Item>::is_empty
 * @brief Return true if the head doesn't point to a node.
 */
template<typename Item>
core_inline
bool List<Item>::is_empty(void) const
{
    return (head_ == nullptr);
}

/**
 * List<Item>::size
 * @brief Return the number of nodes in the list
 */
template<typename Item>
core_inline
size_t List<Item>::size(void) const
{
    return count_;
}

/**
 * List<Item>::clear
 * @brief Delete all the nodes and reset the head and tail pointers.
 */
template<typename Item>
core_inline
void List<Item>::clear(void)
{
    while (!is_empty()) {
        pop_front();
    }
}

/** ---- List API mutator functions -------------------------------------------
 * List<Item>::push_front
 * @brief Create a new node at the head of the list.
 * If list is empty, update head and tail to point to the new node.
 * Otherwise, insert the node before the head.
 */
template<typename Item>
core_inline
void List<Item>::push_front(const Item &item)
{
    DoublyNode<Item> *node = nullptr;
    try {
        node = atto::core::align_alloc<DoublyNode<Item>>(item);
    } catch (std::exception& e) {
        core_throw(e.what());
    }

    if (is_empty()) {
        tail_ = node;
    } else {
        insert_before(head_, node);
    }
    head_ = node;

    ++count_;
}

/**
 * List<Item>::push_back
 * @brief Create a new node at the tail of the list.
 * If list is empty, update head and tail to point to the new node.
 * Otherwise, insert the node after the head.
 */
template<typename Item>
core_inline
void List<Item>::push_back(const Item &item)
{
    DoublyNode<Item> *node = nullptr;
    try {
        node = atto::core::align_alloc<DoublyNode<Item>>(item);
    } catch (std::exception& e) {
        core_throw(e.what());
    }

    if (is_empty()) {
        head_ = node;
    } else {
        insert_after(tail_, node);
    }
    tail_ = node;

    ++count_;
}

/**
 * List<Item>::pop_front
 * @brief Delete a node from the front of the list.
 */
template<typename Item>
core_inline
void List<Item>::pop_front(void)
{
    core_assert(!is_empty(), "empty list, out of range error");
    erase(head_);
    --count_;
}

/**
 * List<Item>::pop_back
 * @brief Delete a node from the front of the list.
 */
template<typename Item>
core_inline
void List<Item>::pop_back(void)
{
    core_assert(!is_empty(), "empty list, out of range error");
    erase(tail_);
    --count_;
}

/** ---- List API accessor functions ------------------------------------------
 * List<Item>::front
 */
template<typename Item>
core_inline
Item &List<Item>::front(void)
{
    core_assert(!is_empty(), "empty list, out of range error");
    return head_->item_;
}

/**
 * List<Item>::front
 */
template<typename Item>
core_inline
const Item &List<Item>::front(void) const
{
    core_assert(!is_empty(), "empty list, out of range error");
    return head_->item_;
}

/**
 * List<Item>::back
 */
template<typename Item>
core_inline
Item &List<Item>::back(void)
{
    core_assert(!is_empty(), "empty list, out of range error");
    return tail_->item_;
}

/**
 * List<Item>::back
 */
template<typename Item>
core_inline
const Item &List<Item>::back(void) const
{
    core_assert(!is_empty(), "empty list, out of range error");
    return tail_->item_;
}

} /* namespace algo */

#endif /* ALGO_DS_LINKED_LIST_H_ */
