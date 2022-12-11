/*
 * array2d.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ALGO_NUMERIC_ARRAY_2D_H_
#define ALGO_NUMERIC_ARRAY_2D_H_

#include "atto/core/core.hpp"

namespace algo {

/** ---- Array2d data structure -----------------------------------------------
 *
 * class Array2d
 *
 * Array2d is a 2-dimensional representation of a contiguous,
 * non resizable block array of n1*n2 numeric elements of type T.
 *
 *  API Array2d<T>:
 *      Array2d(n1,n2)             create a matrix with n1*n2 elements
 *      ~Array2d()                 destroy the matrix
 *
 *      size_t size()               number of items in the matrix
 *      size_t n1()                 number of items in the first dimension
 *      size_t n2()                 number of items in the second dimension
 *
 *      T &operator()(i,j)         access indexed item(i,j)
 *      const T &operator()(i,j)   const access indexed item(i,j)
 *
 *      T *data()                  access the data memory block
 *      const T *data()            const access the data memory block
 *
 *      void read(fp)              read matrix from input file
 *      void write(fp)             write matrix to output file
 */
template<typename T>
struct Array2d {
    static_assert(std::is_arithmetic<T>::value, "non arithmetic type");

public:
    /*
     * Array2d public API member functions.
     */
    size_t size(void) const { return n1_*n2_; }
    size_t n1(void) const { return n1_; }
    size_t n2(void) const { return n2_; }

    T *data() { return *data_.get(); }
    const T *data() const { return *data_.get(); }

    T &operator()(size_t i, size_t j) {
        return (*data_.get())[i*n2_ + j];
    }
    const T &operator()(size_t i, size_t j) const {
        return (*data_.get())[i*n2_ + j];
    }

    void read(atto::core::FileIn &fp);
    void write(atto::core::FileOut &fp);

    /*
     * Array2d constructor/destructor.
     */
    explicit Array2d(size_t n1, size_t n2, T value = static_cast<T>(0))
        : data_(
            std::move(atto::core::make_align_unique_array<T>(n1*n2, value)))
        , n1_(n1)
        , n2_(n2) {}
    ~Array2d() = default;
    /*
     * Array2d copy constructor/assignment.
     */
    Array2d(const Array2d &other)
        : data_(
            std::move(atto::core::make_align_unique_array<T>(other.size())))
        , n1_(other.n1_)
        , n2_(other.n2_) {
            std::memcpy(data(), other.data(), other.size()*sizeof(T));
        }
    Array2d &operator=(const Array2d &other) {
        if (this == &other) {
            return *this;
        }

        data_ = std::move(atto::core::make_align_unique_array<T>(other.size()));
        std::memcpy(data(), other.data(), other.size()*sizeof(T));

        n1_ = other.n1_;
        n2_ = other.n2_;

        return *this;
    }
    /*
     * Array2d move constructor/assignment.
     */
    Array2d(Array2d &&other)
        : data_(std::move(other.data_))
        , n1_(std::move(other.n1_))
        , n2_(std::move(other.n2_)) {
        /* Reset the other matrix. */
        other.data_.reset(nullptr);
        other.n1_ = 0;
        other.n2_ = 0;
    }
    Array2d &operator=(Array2d &&other) {
        if (this == &other) {
            return *this;
        }

        /* Move the other matrix. */
        data_ = std::move(other.data_);
        n1_ = std::move(other.n1_);
        n2_ = std::move(other.n2_);

        /* Reset the other matrix. */
        other.data_.reset(nullptr);
        other.n1_ = 0;
        other.n2_ = 0;

        return *this;
    }

private:
    /*
     * Tensor private member variables.
     */
    atto::core::align_unique_array<T> data_;  /* matrix memory block */
    size_t n1_;                         /* matrix dimensions */
    size_t n2_;
};

/**
 * Array2d<T>::read
 *
 * Read the matrix elements from input stream
 * using the appropriate format specifier.
 */
template<typename T>
void Array2d<T>::read(atto::core::FileIn &fp)
{
    /* Set the input format of type T and read the stream */
    std::string fmt = atto::core::str_read_format<T>();

    for (size_t i = 0; i < n1_; ++i) {
        for (size_t j = 0; j < n2_; ++j) {
            core_assert(
                fp.scanf(fmt.c_str(), &(*this)(i, j)) == 1,
                 "failed to read matrix");
        }
    }
}

/**
 * Array2d<T>::write
 *
 * Write the matrix elements to output stream
 * using the appropriate format specifier.
 */
template<typename T>
void Array2d<T>::write(atto::core::FileOut &fp)
{
    /* Set the output format of type T and write the stream */
    std::string fmt = atto::core::str_write_format<T>();

    for (size_t i = 0; i < n1_; ++i) {
        for (size_t j = 0; j < n2_; ++j) {
            core_assert(
                fp.printf(" ") > 0 && fp.printf(fmt.c_str(), (*this)(i, j)) > 0,
                "failed to write matrix");
        }
        core_assert(fp.printf("\n") > 0, "failed to write matrix");
    }
}

} /* namespace algo */

#endif /* ALGO_NUMERIC_ARRAY_2D_H_ */
