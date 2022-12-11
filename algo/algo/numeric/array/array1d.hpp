/*
 * array1d.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ALGO_NUMERIC_ARRAY_1D_H_
#define ALGO_NUMERIC_ARRAY_1D_H_

#include "atto/core/core.hpp"

namespace algo {

/** ---- Array1d data structure -----------------------------------------------
 *
 * class Array1d
 *
 * Array1d is a 1-dimensional representation of a contiguous,
 * non resizable block array of n1 numeric elements of type T.
 *
 *  API Array1d<T>:
 *      Array1d(n1)                create a vector with n1 elements
 *      ~Array1d()                 destroy the vector
 *
 *      size_t size()               number of items in the vector
 *      size_t n1()                 number of items in the first dimension
 *
 *      T &operator()(i)           access indexed item(i,j,k)
 *      const T &operator()(i)     const access indexed item(i,j,k)
 *
 *      T *data()                  access the data memory block
 *      const T *data()            const access the data memory block
 *
 *      void read(fp)              read vector from input file
 *      void write(fp)             write vector to output file
 */
template<typename T>
class Array1d {
    static_assert(std::is_arithmetic<T>::value, "non arithmetic type");

public:
    /*
     * Array1d public API member functions.
     */
    size_t size(void) const { return n1_; }
    size_t n1(void) const { return n1_; }

    T *data() { return *data_.get(); }
    const T *data() const { return *data_.get(); }

    T &operator()(size_t i) { return (*data_.get())[i]; }
    const T &operator()(size_t i) const { return (*data_.get())[i]; }

    void read(atto::core::FileIn &fp);
    void write(atto::core::FileOut &fp);

    /*
     * Array1d constructor/destructor.
     */
    explicit Array1d(size_t n1, T value = static_cast<T>(0))
        : data_(
            std::move(atto::core::make_align_unique_array<T>(n1, value)))
        , n1_(n1) {}
    ~Array1d() = default;
    /*
     * Array1d copy constructor/assignment.
     */
    Array1d(const Array1d &other)
        : data_(
            std::move(atto::core::make_align_unique_array<T>(other.size())))
        , n1_(other.n1_) {
            std::memcpy(data(), other.data(), other.size()*sizeof(T));
        }
    Array1d &operator=(const Array1d &other) {
        if (this == &other) {
            return *this;
        }

        data_ = std::move(atto::core::make_align_unique_array<T>(other.size()));
        std::memcpy(data(), other.data(), other.size()*sizeof(T));

        n1_ = other.n1_;

        return *this;
    }
    /*
     * Array1d move constructor/assignment.
     */
    Array1d(Array1d &&other)
        : data_(std::move(other.data_))
        , n1_(std::move(other.n1_)) {
        /* Reset the other vector. */
        other.data_.reset(nullptr);
        other.n1_ = 0;
    }
    Array1d &operator=(Array1d &&other) {
        if (this == &other) {
            return *this;
        }

        /* Move the other vector. */
        data_ = std::move(other.data_);
        n1_ = std::move(other.n1_);

        /* Reset the other vector. */
        other.data_.reset(nullptr);
        other.n1_ = 0;

        return *this;
    }

private:
    /*
     * Array1d private member variables.
     */
    atto::core::align_unique_array<T> data_;  /* vector memory block */
    size_t n1_;                         /* vector dimensions */
};

/**
 * Array1d<T>::read
 *
 * Read the vector elements from input stream
 * using the appropriate format specifier.
 */
template<typename T>
void Array1d<T>::read(atto::core::FileIn &fp)
{
    /* Set the input format of type T and read the stream */
    std::string fmt = atto::core::str_read_format<T>();

    for (size_t i = 0; i < n1_; ++i) {
        core_assert(
            fp.scanf(fmt.c_str(), &(*this)(i)) == 1,
            "failed to read vector");
    }
}

/**
 * Array1d<T>::write
 *
 * Write the vector elements to output stream
 * using the appropriate format specifier.
 */
template<typename T>
void Array1d<T>::write(atto::core::FileOut &fp)
{
    /* Set the output format of type T and write the stream */
    std::string fmt = atto::core::str_write_format<T>();

    for (size_t i = 0; i < n1_; ++i) {
        core_assert(
            fp.printf(" ") > 0 && fp.printf(fmt.c_str(), (*this)(i)) > 0,
            "failed to write vector");
    }
    core_assert(fp.printf("\n") > 0, "failed to write vector");
}

} /* namespace algo */

#endif /* ALGO_NUMERIC_ARRAY_1D_H_ */
