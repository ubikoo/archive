/*
 * kdtree.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ALGO_SEARCH_KDTREE_STATIC_H_
#define ALGO_SEARCH_KDTREE_STATIC_H_

#include "atto/core/core.hpp"
#include <queue>
#include <iomanip>

#define KDTREE_T_DECL template<typename Type, std::size_t Dim>
#define KDTREE_C_DECL KdTreeStatic<Type, Dim>

namespace algo {

/** ---- kd-search tree class (static) ----------------------------------------
 *
 * class KdTreeStatic
 * @brief KdTreeStatic(k-dimensional search tree) represents a space
 * partitioning binary search tree where each node is associated with one
 * of the k-dimensions, with the hyperplane perpendicular to the k-axis.
 * The node dimension axis is given by its depth modulo k.
 *
 * Each non-leaf node implicitly divides the space into two half-spaces,
 * split by a hyperplane along a given dimension.
 * Points left of the hyperplane are represented by the left subtree and
 * points right of the hyperplane are represented by the right subtree of
 * the node.
 *
 * Each node contains a k-dimensional point, an axis aligned bounding box
 * representing the space partitioned by the point, a left link, a right
 * link and size of the subtree rooted at the node.
 *
 * @note
 * KdTreeStatic underlying data structure is represented by a vector of
 * nodes held by a unique pointer.
 * The data structure is static and the class does not allow removal of
 * node points from an existing tree.
 * This makes the static variant a more efficient data structure to
 * perform search queries over a collection of points.
 *
 *  API KdTreeStatic<Type,Dim>:
 *      KdTreeStatic()             create an empty kd-tree.
 *      KdTreeStatic(points)       create a kd-tree from an array of points.
 *      ~KdTreeStatic()            remove all points and destroy the tree.
 *
 *      size_t size(void) const    return the size of the tree.
 *      bool is_empty(void) const  is the binary search tree empty?
 *      void clear(void)           clear all points in the tree.
 *
 *      void insert(point)         insert a point in the tree.
 *      void build(points)         build the tree from an array of points
 *
 *      bool contains(point)       does the tree contain the point?
 *
 *      const point &min(dim)      return the minimum point in the tree
 *                                  along a given dimension.
 *      const point &max(dim)      return the maximum point in the tree
 *                                  along a given dimension.
 *
 *      vector<points> knn_query(center, knn)
 *                                  return the k-nearest-neighbours from
 *                                  the query point.
 *      vector<points> sphere_query(center, radius)
 *                                  return the neighbours within a ball of
 *                                  size(Euclidean radius distance),
 *                                  centered at the query point.
 *      vector<points> range_query(range_lo, range_hi)
 *                                  return the number of neighbours within
 *                                  an axis aligned bounding box.
 *      vector<points> points_query(void)
 *                                  return all the points in the kd-tree.
 *
 * @see
 * Algorithms 4th Edition by Robert Sedgewick, Kevin Wayne.
 */
template<typename Type, std::size_t Dim>
class KdTreeStatic {
    /*
     * KdTreeStatic only works with floating point data types.
     */
    static_assert(
        std::is_floating_point<Type>::value, "non floating point numeric type");

    /*
     * KdTreeStatic forward declarations.
     */
    struct Point;       /* kd-tree point class */
    struct Rect;        /* kd-tree rectangle class. */
    struct Node;        /* kd-tree node class. */

    /*
     * KdTreeStatic typedef declarations.
     */
    typedef std::array<Type, Dim>           array_type;
    typedef std::vector<array_type>         vector_array_type;

    typedef std::pair<const Node *, Type>   pair_type;
    typedef std::vector<pair_type>          vector_pair_type;

    struct node_compare {
        bool operator()(const pair_type &lhs, const pair_type &rhs) {
            return (lhs.second < rhs.second);
        }
    };
    typedef std::priority_queue<
        pair_type,
        vector_pair_type,
        node_compare> queue_type;

public:
    /* --- KdTreeStatic public API member functions ---------------------------
     *
     * Return the number of points in the kd-tree.
     */
    size_t size(void) const {
        return size(root_);
    }
    /*
     * Does the kd-tree contain any point?
     */
    bool is_empty(void) const {
        return (size() == 0);
    }
    /*
     * Remove all points from the kd-tree.
     */
    void clear(void) {
        tree_.clear();
        root_ = nullptr;
    }
    /*
     * Insert a point represented by its coordinates into the kd-tree.
     * Do nothing if the kd-tree already contains the specified point.
     */
    void insert(const array_type &array) {
        Point point(array);
        if (contains(root_, point)) {
            return;
        }

        Point qlo, qhi;
        for (size_t i = 0; i < Dim; ++i) {
            qlo[i] = -std::numeric_limits<Type>::max();
            qhi[i] =  std::numeric_limits<Type>::max();
        }
        Rect cell(qlo, qhi);
        root_ = insert(root_, point, cell, 0);
    }
    /*
     * Build a kd-tree from a collection of points.
     */
    void build(const vector_array_type &data) {
        clear();
        for (const auto &q : data) {
            insert(q);
        }
    }
    /*
     * Does the kd-tree contain the given point?
     */
    bool contains(const array_type &array) const {
        Point point(array);
        return contains(root_, point);
    }
    /*
     * Return the minimum point from the kd-tree.
     */
    const array_type &min(size_t dim) const {
        core_assert(!is_empty(), "invalid minimum on empty kd-tree");
        const Node *node = min(root_, dim);
        return node->point_.coord_;
    }
    /*
     * Return the maximum point from the kd-tree.
     */
    const array_type &max(size_t dim) const {
        core_assert(!is_empty(), "invalid maximum on empty kd-tree");
        const Node *node = max(root_, dim);
        return node->point_.coord_;
    }
    /*
     * Return the k-nearest-neighbours from the query point.
     */
    vector_array_type knn_query(
        const array_type &center,
        size_t knn) const {
        core_assert(!is_empty(), "invalid query on empty kd-tree");

        Point point(center);
        queue_type result;
        knn_query(root_, point, knn, result);

        vector_array_type points;
        while (!result.empty()) {
            auto item = result.top();
            const Node *node = item.first;
            points.push_back(node->point_.coord_);
            result.pop();
        }
        return points;
    }
    /*
     * Return the neighbours within a ball of size(Euclidean
     * radius distance), centered at the query point.
     */
    vector_array_type sphere_query(
        const array_type &center,
        Type radius) const {
        core_assert(!is_empty(), "invalid query on empty kd-tree");

        Point point(center);
        vector_array_type points;
        sphere_query(root_, point, radius, points);
        return points;
    }
    /*
     * Return the number of neighbours within an axis aligned
     * bounding box given by the low and high query points,
     * range_lo and range_hi.
     */
    vector_array_type range_query(
        const array_type &range_lo,
        const array_type &range_hi) const {
        core_assert(!is_empty(), "invalid query on empty kd-tree");

        Point qlo(range_lo);
        Point qhi(range_hi);
        Rect rect(qlo, qhi);

        vector_array_type points;
        range_query(root_, rect, points);
        return points;
    }
    /*
     * Return all the points in the tree by running a range
     * query over an axis aligned bounding box given by the
     * lowest and largest values for the floating type.
     */
    vector_array_type points_query(void) const {
        const Type minval = -std::numeric_limits<Type>::max();
        const Type maxval =  std::numeric_limits<Type>::max();

        const array_type range_lo = {minval, minval};
        const array_type range_hi = {maxval, maxval};

        return range_query(range_lo, range_hi);
    }

    /** ---- KdTreeStatic special member functions -----------------------------
     *
     * KdTreeStatic constructor/destructor.
     */
    explicit KdTreeStatic() : root_(nullptr) {}
    explicit KdTreeStatic(const vector_array_type &points)
        : root_(nullptr) {
        try {
            build(points);
        } catch (std::exception& e) {
            clear();
            core_throw(e.what());
        }
    }
    ~KdTreeStatic() { clear(); }
    /*
     * KdTreeStatic copy constructor/assignment.
     */
    KdTreeStatic(const KdTreeStatic &other) : root_(nullptr) {
        try {
            build(other.points_query());
        } catch (std::exception& e) {
            clear();
            core_throw(e.what());
        }
    }
    KdTreeStatic &operator=(const KdTreeStatic &other) {
        if (this == &other)
            return *this;

        try {
            build(other.points_query());
        } catch (std::exception& e) {
            clear();
            core_throw(e.what());
        }

        return *this;
    }

private:
    /** ---- KdTreeStatic private API member functions -------------------------
     *
     * KdTreeStatic<Type,Dim>::size
     */
    size_t size(const Node *node) const;
    /*
     * KdTreeStatic<Type,Dim>::insert
     */
    Node *insert(
        Node *node,
        const Point &point,
        const Rect &cell,
        size_t cutdim);
    /*
     * KdTreeStatic<Type,Dim::contains
     */
    bool contains(Node *node, const Point &point) const;
    /*
     * KdTreeStatic<Type,Dim>::min
     */
    const Node *min(Node *node, size_t dim) const;
    /*
     * KdTreeStatic<Type,Dim>::max
     */
    const Node *max(Node *node, size_t dim) const;
    /*
     * KdTreeStatic<Type,Dim>::nodemin
     */
    const Node *nodemin(const Node *lhs, const Node *rhs, size_t dim) const;
    /*
     * KdTreeStatic<Type,Dim>::nodemax
     */
    const Node *nodemax(const Node *lhs, const Node *rhs, size_t dim) const;
    /*
     * KdTreeStatic<Type,Dim>::knn_query
     */
    void knn_query(
        const Node *node,
        const Point &point,
        size_t knn,
        queue_type &result) const;
    /*
     * KdTreeStatic<Type,Dim>::sphere_query
     */
    void sphere_query(
        const Node *node,
        const Point &point,
        Type radius,
        vector_array_type &result) const;
    /*
     * KdTreeStatic<Type,Dim>::range_query
     */
    void range_query(
        const Node *node,
        const Rect &rect,
        vector_array_type &result) const;

    /** ---- KdTreeStatic private member variables -----------------------------
     */
    Node *root_;                                /* kd-tree root link */
    std::vector<std::unique_ptr<Node>> tree_;   /* kd-tree data structure */
}; /* class KdTreeStatic */


/** ---- KdTreeStatic private API member functions ----------------------------
 *
 * KdTreeStatic<Type,Dim>::size
 * @brief Return the total number of points in the tree.
 */
KDTREE_T_DECL
inline
size_t KDTREE_C_DECL::size(const Node *node) const
{
    if (node == nullptr) {
        return 0;
    }
    return node->size_;
}

/**
 * KdTreeStatic<Type,Dim>::build
 * @brief Build a kd-tree from an array of points.
 */
KDTREE_T_DECL
inline
typename KDTREE_C_DECL::Node *KDTREE_C_DECL::insert(
    Node *node,
    const Point &point,
    const Rect &cell,
    size_t cutdim)
{
    /*
     * Fell out of the tree, create new leaf.
     */
    if (node == nullptr) {
        tree_.push_back(std::make_unique<Node>(point, cell, cutdim, 1));
        return tree_.back().get();
    }

    /*
     * If the node point is a duplicate with equal coordinates,
     * do nothing and just return the current node.
     *
     * If the point coordinate along the node cutting dimension
     * is less than the node point coordinate, insert the point
     * on the left subtree.
     *
     * Otherwise, insert the point on the right subtree.
     */
    if (point.is_equal(node->point_)) {
        return node;
    } else if (point.is_less(node->point_, node->cutdim_)) {
        node->left_ = insert(
            node->left_,
            point,
            node->left_cell(),
           (node->cutdim_ + 1) % Dim);
    } else {
        node->right_ = insert(
            node->right_,
            point,
            node->right_cell(),
           (node->cutdim_ + 1) % Dim);
    }

    node->size_ = 1 + size(node->left_) + size(node->right_);
    return node;
}

/**
 * KdTreeStatic<Type,Dim>::contains
 * @brief Does the kd-tree contain the given point?
 */
KDTREE_T_DECL
inline
bool KDTREE_C_DECL::contains(Node *node, const Point &point) const
{
    if (node == nullptr) {
        return false;
    }

    if (point.is_equal(node->point_)) {
        return true;
    } else if (point.is_less(node->point_, node->cutdim_)) {
        return contains(node->left_, point);
    } else {
        return contains(node->right_, point);
    }
}

/**
 * KdTreeStatic<Type,Dim>::min
 * @brief Return the minimum point in the kd-tree along a given dimension.
 */
KDTREE_T_DECL
inline
const typename KDTREE_C_DECL::Node *KDTREE_C_DECL::min(
    Node *node,
    size_t dim) const
{
    /*
     * Fell out of the tree? How?
     */
    if (node == nullptr) {
        return nullptr;
    }

    /*
     * If the node cutting dimension matches the search cutting dimension,
     * we only need to search on the left subtree. If the node is a leaf,
     * we found the minimum. Otherwise, search along its left subtree.
     *
     * If the node cutting dimension does not match the search cutting
     * dimension, then we cannot infer which subtree contains the smallest
     * point because the node orders the points along a dimension different
     * from the search cutting dimension.
     * We search both left and right subtrees and return the minimum node.
     */
    if (node->cutdim_ == dim) {
        if (node->left_ == nullptr) {
            return node;
        } else {
            return min(node->left_, dim);
        }
    } else {
        const Node *left = min(node->left_, dim);
        const Node *right = min(node->right_, dim);
        return nodemin(nodemin(node, left, dim), right, dim);
    }
}

/**
 * KdTreeStatic<Type,Dim>::max
 * @brief Return the maximum point in the kd-tree along a given dimension.
 */
KDTREE_T_DECL
inline
const typename KDTREE_C_DECL::Node *KDTREE_C_DECL::max(
    Node *node,
    size_t dim) const
{
    /*
     * Fell out of the tree? How?
     */
    if (node == nullptr) {
        return nullptr;
    }

    /*
     * If the node cutting dimension matches the search cutting dimension,
     * we only need to search on the right subtree. If the node is a leaf,
     * we found the maximum. Otherwise, search along its right subtree.
     *
     * If the node cutting dimension does not match the search cutting
     * dimension, then we cannot infer which subtree contains the largest
     * point because the node orders the points along a dimension different
     * from the search cutting dimension.
     * We search both left and right subtrees and return the maximum node.
     */
    if (node->cutdim_ == dim) {
        if (node->right_ == nullptr) {
            return node;
        } else {
            return max(node->right_, dim);
        }
    } else {
        const Node *left = max(node->left_, dim);
        const Node *right = max(node->right_, dim);
        return nodemax(nodemax(node, left, dim), right, dim);
    }
}

/**
 * KdTreeStatic<Type,Dim>::nodemin
 * @brief Return the node containing the minimum point along a specified
 * dimension. The right hand side node can be null, which happens
 * when one attempts to extract the minimum from an empty tree.
 */
KDTREE_T_DECL
inline
const typename KDTREE_C_DECL::Node *KDTREE_C_DECL::nodemin(
    const Node *lhs,
    const Node *rhs,
    size_t dim) const
{
    if (rhs == nullptr) {
        return lhs;
    }

    if (lhs->point_.is_less(rhs->point_, dim)) {
        return lhs;
    }
    return rhs;
}

/**
 * KdTreeStatic<Type,Dim>::nodemax
 * @brief Return the node containing the maximum point along a specified
 * dimension. The right hand side node can be null, which happens
 * when one attempts to extract the maximum from an empty tree.
 */
KDTREE_T_DECL
inline
const typename KDTREE_C_DECL::Node *KDTREE_C_DECL::nodemax(
    const Node *lhs,
    const Node *rhs,
    size_t dim) const
{
    if (rhs == nullptr) {
        return lhs;
    }

    if (lhs->point_.is_greater(rhs->point_, dim)) {
        return lhs;
    }
    return rhs;
}

/**
 * KdTreeStatic<Type,Dim>::knn_query
 * @brief Search the tree for the k-nearest-neighbours to the query point.
 */
KDTREE_T_DECL
inline
void KDTREE_C_DECL::knn_query(
    const Node *node,
    const Point &point,
    size_t knn,
    queue_type &result) const
{
    /*
     * Fell off the tree.
     */
    if (node == nullptr) {
        return;
    }

    /*
     * Get the distance to this node point and add it to the queue.
     *
     * The nearest-neighbour queue maintains a collection of nodes
     * with the knn-smallest distances to the query point.
     * These are ordered in stricly decreasing order, with the node
     * with the largest distance on top.
     *
     * If the queue size is less than knn, just add the new node.
     * If the queue is full, check whether the new node needs to
     * be added. If the new distance is smaller than the largest
     * distance in the queue(top node), pop the top node and push
     * the new node.
     * Otherwise, leave the queue intact to maintain the invariant
     * of having the knn-closest nodes to the query point.
     */
    Type dist = point.dist(node->point_);
    if (result.size() < knn) {
        result.push(std::make_pair(node, dist));
    } else {
        pair_type furthest = result.top();
        if (dist < furthest.second) {
            result.pop();
            result.push(std::make_pair(node, dist));
        }
    }

    /*
     * Search the subtrees rooted at this node for possibly
     * closer points.
     *
     * Check first the subtree where the query point lies along
     * the node cutting dimension since it is more likely to
     * yield a nearest neighbour.
     *
     * If the minimal distance from the query point to the other
     * subtree is larger than the current largest distance then
     * we are guaranteed that the any nodes on the other subtree
     * are even further and we can skip it from the search.
     */
    if (point.is_less(node->point_, node->cutdim_)) {
        /* Query point lies on left subtree */
        knn_query(node->left_, point, knn, result);

        /* Worth visiting the right subtree? */
        pair_type furthest = result.top();
        if (node->right_ &&
            node->right_->cell_.dist(point) < furthest.second) {
            knn_query(node->right_, point, knn, result);
        }
    } else {
        /* Query point lies on right subtree */
        knn_query(node->right_, point, knn, result);

        /* Worth visiting the left subtree? */
        pair_type furthest = result.top();
        if (node->left_ &&
            node->left_->cell_.dist(point) < furthest.second) {
            knn_query(node->left_, point, knn, result);
        }
    }
}

/**
 * KdTreeStatic<Type,Dim>::sphere_query
 * @brief Search the tree for the nodes within a ball of size
 * (Euclidean radius distance) of the query point.
 */
KDTREE_T_DECL
inline
void KDTREE_C_DECL::sphere_query(
    const Node *node,
    const Point &point,
    Type radius,
    vector_array_type &result) const
{
    /*
     * Fell off the tree.
     */
    if (node == nullptr) {
        return;
    }

    /*
     * Get the distance to this node point and add it to the queue.
     *
     * The nearest-neighbour queue maintains a collection of nodes
     * within an Euclidean distance to the query point smaller than
     * the ball radius.
     * These are ordered in stricly decreasing order, with the node
     * with the largest distance on top.
     */
    Type dist = point.dist(node->point_);
    if (dist < radius) {
        result.push_back(node->point_.coord_);
    }

    /*
     * Search the subtrees rooted at this node for possibly
     * closer points.
     *
     * Check first the subtree where the query point lies along
     * the node cutting dimension since it is more likely to
     * yield a nearest neighbour.
     *
     * If the minimal distance from the query point to the other
     * subtree is larger than the ball radius size, then we are
     * guaranteed that the any nodes on the other subtree are
     * even further and we can skip it from the search.
     */
    if (point.is_less(node->point_, node->cutdim_)) {
        /* Query point lies on left subtree */
        sphere_query(node->left_, point, radius, result);

        /* Worth visiting the right subtree? */
        if (node->right_ &&
            node->right_->cell_.dist(point) < radius) {
            sphere_query(node->right_, point, radius, result);
        }
    } else {
        /* Query point lies on right subtree */
        sphere_query(node->right_, point, radius, result);

        /* Worth visiting the left subtree? */
        if (node->left_ &&
            node->left_->cell_.dist(point) < radius) {
            sphere_query(node->left_, point, radius, result);
        }
    }
}

/**
 * KdTreeStatic<Type,Dim>::range_query
 * @brief Search the tree for the nodes within a ball of size
 * (Euclidean radius distance) of the query point.
 */
KDTREE_T_DECL
inline
void KDTREE_C_DECL::range_query(
    const Node *node,
    const Rect &rect,
    vector_array_type &result) const
{
    /*
     * Fell off the tree.
     */
    if (node == nullptr) {
        return;
    }

    /*
     * Add the node point to the queue if it lies within the query
     * range.
     * The nearest-neighbour queue maintains a collection of nodes
     * within the query range.
     */
    if (rect.contains(node->point_)) {
        result.push_back(node->point_.coord_);
    }

    /*
     * Search the subtrees rooted at this node for points
     * lying within the query range.
     *
     * If the query range overlap with the left or right subtrees,
     * search them for points lying within the query range.
     */
    if (node->left_ && !rect.disjoint(node->left_->cell_)) {
        range_query(node->left_, rect, result);
    }

    if (node->right_ && !rect.disjoint(node->right_->cell_)) {
        range_query(node->right_, rect, result);
    }
}

/** ---- KdTreeStatic::Point point class --------------------------------------
 *
 * struct KdTreeStatic<Type,Dim>::Point
 */
template<typename Type, std::size_t Dim>
struct KdTreeStatic<Type,Dim>::Point {
    /* Point coordinates. */
    array_type coord_;

    /** ---- Point API member functions ----------------------------------------
     *
     * Return a reference to a point coordinate along a given dimension.
     */
    Type &operator[](size_t dim) { return coord_[dim]; }
    const Type &operator[](size_t dim) const { return coord_[dim]; }
    /*
     * Primitive type floating-point comparison functions.
     * Based on "The Art of Computer Programming", Donald Knuth.
     */
    bool is_equal(Type u, Type v) const;
    bool is_less(Type u, Type v) const;
    bool is_greater(Type u, Type v) const;
    /*
     * Point type floating-point comparison functions along a cutting dimension.
     * If dim is equal to default value -1, then this point is stricly orderered
     * with the rhs along all dimensions.
     */
    bool is_equal(const Point &rhs, size_t dim = -1) const;
    bool is_less(const Point &rhs, size_t dim = -1) const;
    bool is_greater(const Point &rhs, size_t dim = -1) const;
    /*
     * Return Euclidean distance from another point.
     */
    Type dist(const Point &rhs) const;

    /** ---- Point special member functions ------------------------------------
     *
     * Point constructor/destructor.
     */
    explicit Point() {}
    explicit Point(const array_type &coord) : coord_(coord) {}
    ~Point() {}
    /*
     * Point copy constructor/assignment.
     */
    Point(const Point &other) : coord_(other.coord_) {}
    Point &operator=(const Point &other) {
        if (this == &other)
            return *this;
        coord_ = other.coord_;
        return *this;
    }
    /*
     * Point move constructor/assignment.
     */
    Point(Point &&other) : coord_(std::move(other.coord_)) {}
    Point &operator=(Point &&other) {
        if (this == &other)
            return *this;
        coord_ = std::move(other.coord_);
        return *this;
    }
}; /* struct KdTreeStatic<Type,Dim>::Point */

/**
 * KdTreeStatic<Type,Dim>::Point::is_equal
 * @brief Is u essentially equal to v?
 * (u ~ v), iif |u-v| <= eps*min(|u|,|v|)
 */
KDTREE_T_DECL
inline
bool KDTREE_C_DECL::Point::is_equal(Type u, Type v) const
{
    const Type epsilon = std::numeric_limits<Type>::epsilon();
    const Type abs_u = std::fabs(u);
    const Type abs_v = std::fabs(v);

    if (std::fabs(u - v) < epsilon) {
        return true;
    }

    return std::fabs(u - v) <= (epsilon * (abs_u < abs_v ? abs_u : abs_v));
}

/**
 * KdTreeStatic<Type,Dim>::Point::is_less
 * @brief Is u less than v?
 * (u < v), iif (v-u) > eps*max(|u|,|v|)
 */
KDTREE_T_DECL
inline
bool KDTREE_C_DECL::Point::is_less(Type u, Type v) const
{
    const Type epsilon = std::numeric_limits<Type>::epsilon();
    const Type abs_u = std::fabs(u);
    const Type abs_v = std::fabs(v);

    return (v - u) > (epsilon * (abs_u > abs_v ? abs_u : abs_v));
}

/**
 * KdTreeStatic<Type,Dim>::Point::is_greater
 * @brief Is u greater than v?
 * (u > v), iif (u-v) > eps*max(|u|,|v|)
 */
KDTREE_T_DECL
inline
bool KDTREE_C_DECL::Point::is_greater(Type u, Type v) const
{
    const Type epsilon = std::numeric_limits<Type>::epsilon();
    const Type abs_u = std::fabs(u);
    const Type abs_v = std::fabs(v);

    return (u - v) > (epsilon * (abs_u > abs_v ? abs_u : abs_v));
}

/**
 * KdTreeStatic<Type,Dim>::Point::is_equal
 * @brief Is this point equal to another point along the cutting dimension?
 */
KDTREE_T_DECL
inline
bool KDTREE_C_DECL::Point::is_equal(const Point &rhs, size_t dim) const
{
    if (dim < Dim) {
        return is_equal((*this)[dim], rhs[dim]);
    }

    for (size_t i = 0; i < Dim; ++i) {
        if (!is_equal((*this)[i], rhs[i])) {
            return false;
        }
    }

    return true;
}

/**
 * KdTreeStatic<Type,Dim>::Point::is_less
 * @brief Is this point less than another point along the cutting dimension?
 */
KDTREE_T_DECL
inline
bool KDTREE_C_DECL::Point::is_less(const Point &rhs, size_t dim) const
{
    if (dim < Dim) {
        return is_less((*this)[dim], rhs[dim]);
    }

    for (size_t i = 0; i < Dim; ++i) {
        if (!is_less((*this)[i], rhs[i])) {
            return false;
        }
    }

    return true;
}

/**
 * KdTreeStatic<Type,Dim>::Point::is_greater
 * @brief Is this point greater than another point along the cutting dimension?
 */
KDTREE_T_DECL
inline
bool KDTREE_C_DECL::Point::is_greater(const Point &rhs, size_t dim) const
{
    if (dim < Dim) {
        return is_greater((*this)[dim], rhs[dim]);
    }

    for (size_t i = 0; i < Dim; ++i) {
        if (!is_greater((*this)[i], rhs[i])) {
            return false;
        }
    }

    return true;
}

/**
 * KdTreeStatic<Type,Dim>::Point::dist
 * @brief Return Euclidean distance between two points.
 */
KDTREE_T_DECL
inline
Type KDTREE_C_DECL::Point::dist(const Point &rhs) const
{
    Type dsq = 0.0;
    for (size_t i = 0; i < Dim; ++i) {
        Type d = (*this)[i] - rhs[i];
        dsq += d*d;
    }
    return std::sqrt(dsq);
}


/** ---- KdTreeStatic::Rect rectangle class -----------------------------------
 *
 * struct KdTreeStatic<Type,Dim>::Rect
 */
template<typename Type, std::size_t Dim>
struct KdTreeStatic<Type,Dim>::Rect {
    /* Rect vertex points. */
    Point lo_;              /* lower vertex point */
    Point hi_;              /* higher vertex point */

    /** ---- Rect API member functions -----------------------------------------
     *
     * Does this rectangle contain the point?
     */
    bool contains(const Point &q) const;

    /* Does this rectangle contain the other rectangle? */
    bool contains(const Rect &c) const;

    /* Is this rectangle disjoint from the other rectangle? */
    bool disjoint(const Rect &c) const;

    /* Minimum distance to point. */
    Type dist(const Point &q) const;

    /* Left part from the hyperplane at point and normal to dim-axis. */
    Rect left(const Point &q, size_t dim) const;

    /* Right part from the hyperplane at point and normal to dim-axis. */
    Rect right(const Point &q, size_t dim) const;

    /** ---- Rect special member functions -------------------------------------
     *
     * Rect constructor/destructor
     */
    explicit Rect(const Point &qlo, const Point &qhi) {
        for (size_t i = 0; i < Dim; ++i) {
            lo_[i] = qlo[i] < qhi[i] ? qlo[i] : qhi[i];
            hi_[i] = qlo[i] < qhi[i] ? qhi[i] : qlo[i];
            core_assert(lo_[i] < hi_[i], "strict inequality violation");
        }
    }
    ~Rect() {}
    /*
     * Rect copy constructor/assignment.
     */
    Rect(const Rect &other)
        : lo_(other.lo_)
        , hi_(other.hi_) {}
    Rect &operator=(const Rect &other) {
        if (this == &other)
            return *this;
        lo_ = other.lo_;
        hi_ = other.hi_;
        return *this;
    }
    /*
     * Rect move constructor/assignment.
     */
    Rect(Rect &&other)
        : lo_(std::move(other.lo_))
        , hi_(std::move(other.hi_)) {}
    Rect &operator=(Rect &&other) {
        if (this == &other)
            return *this;
        lo_ = std::move(other.lo_);
        hi_ = std::move(other.hi_);
        return *this;
    }
}; /* struct KdTreeStatic<Type,Dim>::Rect */

/**
 * KdTreeStatic<Type,Dim>::Rect::contains
 * @brief Does this rectangle contain the point?
 * A point lies within the rectangle if:
 *
 *  lo[i] <= q[i] <= hi[i],  for all 0 <= i <= dim-1
 */
KDTREE_T_DECL
inline
bool KDTREE_C_DECL::Rect::contains(const Point &q) const
{
    for (size_t i = 0; i < Dim; ++i) {
        if (q[i] < lo_[i] || q[i] > hi_[i]) {
            return false;
        }
    }
    return true;
}

/**
 * KdTreeStatic<Type,Dim>::Rect::contains
 * @brief Does this rectangle contain the other rectangle?
 * A rectangle c lies within the rectangle if:
 *
 *  lo[i] <= c.lo[i] <= hi[i],  for all 0 <= i <= dim-1
 *  lo[i] <= c.hi[i] <= hi[i],  for all 0 <= i <= dim-1
 */
KDTREE_T_DECL
inline
bool KDTREE_C_DECL::Rect::contains(const Rect &c) const
{
    for (size_t i = 0; i < Dim; ++i) {
        if (c.lo_[i] < lo_[i] || c.hi_[i] > hi_[i]) {
            return false;
        }
    }
    return true;
}

/**
 * KdTreeStatic<Type,Dim>::Rect::disjoint
 * @brief Is this rectangle disjoint from the other rectangle.
 * A rectangle c is disjoint from the rectangle if:
 *
 *  hi[i] <= c.lo[i]  or  lo[i] >= c.hi[i],  for all 0 <= i <= dim-1
 */
KDTREE_T_DECL
inline
bool KDTREE_C_DECL::Rect::disjoint(const Rect &c) const
{
    for (size_t i = 0; i < Dim; ++i) {
        if (c.lo_[i] > hi_[i] || c.hi_[i] < lo_[i]) {
            return true;
        }
    }
    return false;
}

/**
 * KdTreeStatic<Type,Dim>::Rect::dist
 * @brief Return the minimum Euclidean distance from q to any point on
 * this rectangle.
 * Compute the square root sum of the distance from coordinate
 * q[i] to the ith defining interval of this rectangle:
 *
 *  sqrt( sum( dist({q[i], lo[i], hi[i]})^2 ) ),
 *
 * {q[i], lo[i], hi[i]} = 0, if (lo[i] <= q[i] <= hi[i])
 * {q[i], lo[i], hi[i]} = min(|q[i] - lo[i]|, |q[i] - hi[i]|), otherwise
 */
KDTREE_T_DECL
inline
Type KDTREE_C_DECL::Rect::dist(const Point &q) const
{
    Type sumdsq = 0.0;
    for (size_t i = 0; i < Dim; ++i) {
        Type d = 0.0;
        if (q[i] < lo_[i] || q[i] > hi_[i]) {
            Type dlo = std::fabs(q[i] - lo_[i]);
            Type dhi = std::fabs(q[i] - hi_[i]);
            d = dlo < dhi ? dlo : dhi;
        }
        sumdsq += d*d;
    }
    return std::sqrt(sumdsq);
}

/**
 * KdTreeStatic<Type,Dim>::Rect::left
 * @brief Left part from the hyperplane at point and normal to dim-axis.
 *
 * Given a cutting dimension dim and a point lying within the rectangle.
 * return the subrectangle left of the hyperplane at point and normal
 * to the cut dimension dim.
 * Formaly, return a subrectangle whose low point is lo and whose high
 * point is hi, except for the dim-th coordinate, given by q[dim].
 */
KDTREE_T_DECL
inline
typename KDTREE_C_DECL::Rect KDTREE_C_DECL::Rect::left(
    const Point &q,
    size_t dim) const
{
    Point qlo = lo_;
    Point qhi = hi_;

    qhi[dim] = q[dim];

    return Rect(qlo, qhi);
}

/**
 * KdTreeStatic<Type,Dim>::Rect::right
 * @brief Right part from the hyperplane at point and normal to dim-axis.
 *
 * Given a cutting dimension dim and a point lying within the rectangle.
 * return the subrectangle right of the hyperplane at point and normal
 * to the cut dimension dim.
 * Formaly, return a subrectangle whose high point is hi and whose low
 * point is low, except for the dim-th coordinate, given by q[dim].
 */
KDTREE_T_DECL
inline
typename KDTREE_C_DECL::Rect KDTREE_C_DECL::Rect::right(
    const Point &q,
    size_t dim) const
{
    Point qlo = lo_;
    Point qhi = hi_;

    qlo[dim] = q[dim];

    return Rect(qlo, qhi);
}


/** ---- KdTreeStatic::Node node class ----------------------------------------
 *
 * struct KdTreeStatic<Type,Dim>::Node
 */
template<typename Type, std::size_t Dim>
struct KdTreeStatic<Type,Dim>::Node {
    /* Node variables */
    Point point_;           /* splitting point */
    Rect cell_;             /* associated cell */
    size_t cutdim_;         /* cutting dimension */
    size_t size_;           /* #nodes in subtree rooted here */
    Node *left_;            /* link to left subtree */
    Node *right_;           /* link to right subtree */

    /** ---- Node API member functions -----------------------------------------
     * Return the left part of the node cell, partitioned by the
     * hyperplane at the node splitting point and normal to the
     * cutting dimension axis.
     */
    Rect left_cell(void) const {
        return cell_.left(point_, cutdim_);
    }
    /*
     * Return the right part of the node cell, partitioned by the
     * hyperplane at the node splitting point and normal to the
     * cutting dimension axis.
     */
    Rect right_cell(void) const {
        return cell_.right(point_, cutdim_);
    }

    /** ---- Node special member functions -------------------------------------
     *
     * Node constructor/destructor.
     */
    explicit Node(
        const Point &point,
        const Rect &cell,
        size_t cutdim,
        size_t size)
        : point_(point)
        , cell_(cell)
        , cutdim_(cutdim)
        , size_(size)
        , left_(nullptr)
        , right_(nullptr) {}
    ~Node() {}
    /*
     * Node copy constructor/assignment.
     */
    Node(const Node &other)
        : point_(other.point_)
        , cell_(other.cell_)
        , cutdim_(other.cutdim_)
        , size_(other.size_)
        , left_(other.left_)
        , right_(other.right_) {}
    Node &operator=(const Node &other) {
        if (this == &other)
            return *this;
        point_ = other.point_;
        cell_ = other.cell_;
        cutdim_ = other.cutdim_;
        size_ = other.size_;
        left_ = other.left_;
        right_ = other.right_;
        return *this;
    }
    /*
     * Node move constructor/assignment.
     */
    Node(Node &&other)
        : point_(std::move(other.point_))
        , cell_(std::move(other.cell_))
        , cutdim_(std::move(other.cutdim_))
        , size_(std::move(other.size_))
        , left_(std::move(other.left_))
        , right_(std::move(other.right_)) {}
    Node &operator=(Node &&other) {
        if (this == &other)
            return *this;
        point_ = std::move(other.point_);
        cell_ = std::move(other.cell_);
        cutdim_ = std::move(other.cutdim_);
        size_ = std::move(other.size_);
        left_ = std::move(other.left_);
        right_ = std::move(other.right_);
        return *this;
    }
}; /* struct KdTreeStatic<Type,Dim>::Node */

} /* namespace algo */

#undef KDTREE_T_DECL
#undef KDTREE_C_DECL

#endif /* ALGO_SEARCH_KDTREE_STATIC_H_ */
