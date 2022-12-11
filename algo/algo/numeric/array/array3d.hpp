/*
 * array3d.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ALGO_NUMERIC_ARRAY_3D_H_
#define ALGO_NUMERIC_ARRAY_3D_H_

#include "atto/core/core.hpp"

namespace algo {

/** ---- Array3d data structure -----------------------------------------------
 *
 * class Array3d
 *
 * Array3d is a 3-dimensional representation of a contiguous,
 * non resizable block array of n1*n2*n3 numeric elements of type T.
 *
 *  API Array3d<T>:
 *      Array3d(n1,n2,n3)          create a tensor with n1*n2*n3 elements
 *      ~Array3d()                 destroy the tensor
 *
 *      size_t size()               number of items in the tensor
 *      size_t n1()                 number of items in the first dimension
 *      size_t n2()                 number of items in the second dimension
 *      size_t n3()                 number of items in the third dimension
 *
 *      T &operator()(i,j,k)       access indexed item(i,j,k)
 *      const T &operator()(i,j,k) const access indexed item(i,j,k)
 *
 *      T *data()                  access the data memory block
 *      const T *data()            const access the data memory block
 *
 *      void read(fp)              read tensor from input file
 *      void write(fp)             write tensor to output file
 */
template<typename T>
class Array3d {
    static_assert(std::is_arithmetic<T>::value, "non arithmetic type");

public:
    /*
     * Array3d public API member functions.
     */
    size_t size(void) const { return n1_*n2_*n3_; }
    size_t n1(void) const { return n1_; }
    size_t n2(void) const { return n2_; }
    size_t n3(void) const { return n3_; }

    T *data() { return *data_.get(); }
    const T *data() const { return *data_.get(); }

    T &operator()(size_t i, size_t j, size_t k) {
        return (*data_.get())[i*n2_*n3_ + j*n3_ + k];
    }
    const T &operator()(size_t i, size_t j, size_t k) const {
        return (*data_.get())[i*n2_*n3_ + j*n3_ + k];
    }

    void read(atto::core::FileIn &fp);
    void write(atto::core::FileOut &fp);

    /*
     * Array3d constructor/destructor.
     */
    explicit Array3d(
        size_t n1,
        size_t n2,
        size_t n3,
        T value = static_cast<T>(0))
        : data_(
            std::move(atto::core::make_align_unique_array<T>(n1*n2*n3, value)))
        , n1_(n1)
        , n2_(n2)
        , n3_(n3) {}
    ~Array3d() = default;
    /*
     * Array3d copy constructor/assignment.
     */
    Array3d(const Array3d &other)
        : data_(
            std::move(atto::core::make_align_unique_array<T>(other.size())))
        , n1_(other.n1_)
        , n2_(other.n2_)
        , n3_(other.n3_) {
            std::memcpy(data(), other.data(), other.size()*sizeof(T));
        }
    Array3d &operator=(const Array3d &other) {
        if (this == &other) {
            return *this;
        }
        data_ = std::move(atto::core::make_align_unique_array<T>(other.size()));
        std::memcpy(data(), other.data(), other.size()*sizeof(T));

        n1_ = other.n1_;
        n2_ = other.n2_;
        n3_ = other.n3_;

        return *this;
    }
    /*
     * Array3d move constructor/assignment.
     */
    Array3d(Array3d &&other)
        : data_(std::move(other.data_))
        , n1_(std::move(other.n1_))
        , n2_(std::move(other.n2_))
        , n3_(std::move(other.n3_)) {
        /* Reset the other tensor. */
        other.data_.reset(nullptr);
        other.n1_ = 0;
        other.n2_ = 0;
        other.n3_ = 0;
    }
    Array3d &operator=(Array3d &&other) {
        if (this == &other) {
            return *this;
        }

        /* Move the other tensor. */
        data_ = std::move(other.data_);
        n1_ = std::move(other.n1_);
        n2_ = std::move(other.n2_);
        n3_ = std::move(other.n3_);

        /* Reset the other tensor. */
        other.data_.reset(nullptr);
        other.n1_ = 0;
        other.n2_ = 0;
        other.n3_ = 0;

        return *this;
    }

private:
    /*
     * Array3d private member variables.
     */
    atto::core::align_unique_array<T> data_;  /* tensor memory block */
    size_t n1_;                         /* tensor dimensions*/
    size_t n2_;
    size_t n3_;
};


/**
 * Array3d<T>::read
 *
 * Read the tensor elements from input stream
 * using the appropriate format specifier.
 */
template<typename T>
void Array3d<T>::read(atto::core::FileIn &fp)
{
    /* Set the input format of type T and read the stream */
    std::string fmt = atto::core::str_read_format<T>();

    for (size_t i = 0; i < n1_; ++i) {
        for (size_t j = 0; j < n2_; ++j) {
            for (size_t k = 0; k < n3_; ++k) {
                core_assert(
                    fp.scanf(fmt.c_str(), &(*this)(i, j, k)) == 1,
                    "failed to read tensor");
            }
        }
    }
}

/**
 * Array3d<T>::write
 *
 * Write the tensor elements to output stream
 * using the appropriate format specifier.
 */
template<typename T>
void Array3d<T>::write(atto::core::FileOut &fp)
{
    /* Set the output format of type T and write the stream */
    std::string fmt = atto::core::str_write_format<T>();

    for (size_t i = 0; i < n1_; ++i) {
        for (size_t j = 0; j < n2_; ++j) {
            for (size_t k = 0; k < n3_; ++k) {
                core_assert(
                    fp.printf(" ") > 0 &&
                    fp.printf(fmt.c_str(), (*this)(i, j, k)) > 0,
                    "failed to write tensor");
            }
            core_assert(fp.printf("\n") > 0, "failed to write tensor");
        }
    }
}

} /* namespace algo */

#endif /* ALGO_NUMERIC_ARRAY_3D_H_ */
