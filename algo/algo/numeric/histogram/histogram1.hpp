/*
 * histogram1.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ALGO_NUMERIC_HISTOGRAM1_H_
#define ALGO_NUMERIC_HISTOGRAM1_H_

#include "atto/core/core.hpp"
#include "atto/math/math.hpp"
#include "numeric/array/array.hpp"

namespace algo {

/** ---- Histogram1 distribution ----------------------------------------------
 * struct Histogram1
 */
struct Histogram1 {
    /* Histogram member variables and access functions. */
    size_t xbins_ = 0;
    double xmin_ = 0.0;
    double xmax_ = 0.0;
    Vector<double> count_;
    Vector<double> value_;

    const double &count(size_t i) const {
        core_assert(i < xbins_, "invalid histogram index");
        return count_(i);
    }

    const double &value(size_t i) const {
        core_assert(i < xbins_, "invalid histogram index");
        return value_(i);
    }

    /* Constructor/destructor. */
    Histogram1(const size_t xbins)
        : xbins_(xbins)
        , count_(Vector<double>(xbins))
        , value_(Vector<double>(xbins)) {}
    Histogram1(const size_t xbins, const double xmin, const double xmax)
        : xbins_(xbins)
        , xmin_(xmin)
        , xmax_(xmax)
        , count_(Vector<double>(xbins))
        , value_(Vector<double>(xbins)) {}
    ~Histogram1() {}

    /* Copy constructor/assignment. */
    Histogram1(const Histogram1 &other)
        : xbins_(other.xbins_)
        , xmin_(other.xmin_)
        , xmax_(other.xmax_)
        , count_(other.count_)
        , value_(other.value_) {}

    Histogram1 &operator=(const Histogram1 &other) {
        if (this == &other) {
            return *this;
        }

        xbins_ = other.xbins_;
        xmin_ = other.xmin_;
        xmax_ = other.xmax_;
        count_ = other.count_;
        value_ = other.value_;

        return *this;
    }

    /* Move constructor/assignment. */
    Histogram1(Histogram1 &&other)
        : xbins_(std::move(other.xbins_))
        , xmin_(std::move(other.xmin_))
        , xmax_(std::move(other.xmax_))
        , count_(std::move(other.count_))
        , value_(std::move(other.value_)) {}

    Histogram1 &operator=(Histogram1 &&other) {
        if (this == &other) {
            return *this;
        }

        xbins_ = std::move(other.xbins_);
        xmin_ = std::move(other.xmin_);
        xmax_ = std::move(other.xmax_);
        count_ = std::move(other.count_);
        value_ = std::move(other.value_);

        return *this;
    }

    /*
     * Histogram data functions.
     */
    size_t size(void) const { return xbins_; }
    void clear(void);

    void set(double xmin, double xmax, bool reset = false);
    void bin(double xval, double weight, bool midpoint = true);
    double integral(void) const;

    /*
     * Histogram input/output.
     */
    void read(atto::core::FileIn &fp);
    void write(atto::core::FileOut &fp, bool normalize = false);
};

/**
 * Histogram1::clear
 * @brief Reset the histogram count and value data.
 */
core_inline
void Histogram1::clear(void)
{
    for (size_t i = 0; i < xbins_; ++i) {
        count_(i) = 0.0;
        value_(i) = 0.0;
    }
}

/**
 * Histogram1::set
 * @brief Set the histogram ranges and reset if needed.
 */
core_inline
void Histogram1::set(double xmin, double xmax, bool reset)
{
    core_assert(xmin < xmax, "invalid range");

    /* Set the histogram ranges */
    xmin_ = xmin;
    xmax_ = xmax;

    /* Reset the histogram */
    if (reset) {
        clear();
    }
}

/**
 * Histogram1::bin
 * @brief Bin a sample value with a given weight.
 */
core_inline
void Histogram1::bin(double xval, double weight, bool midpoint)
{
    /*
     * Compute the bin of xval.
     * Choose between the lower and the midpoint rounding conventions.
     *
     * Lower convention takes the bin of x to be
     * [x_k, x_k + dx], i.e., k = floor((x-x_0)/dx).
     *
     * Midpoint convention takes the bin of x to be
     * [x_k - dx/2, x_k + dx/2], i.e.,
     * k = floor((x-(x0-dx/2)/dx) = floor((x-x0)/dx + dx/2).
     */
    double bias = midpoint ? 0.5 : 0.0;
    double ux = (double) xbins_ * (xval - xmin_) / (xmax_ - xmin_);
    size_t ix = std::floor(ux + bias);

    /* Increment the bin counter and bin value. */
    ix = atto::math::fclamp<size_t>(0, xbins_-1, ix);
    count_(ix) += 1.0;
    value_(ix) += weight;
}

/**
 * Histogram1::integral
 * @brief Compute the histogram probability integral.
 */
core_inline
double Histogram1::integral(void) const
{
    double count = 0.0;
    for (size_t i = 0; i < xbins_; ++i) {
        count += count_(i);
    }

    double xdel = (xmax_ - xmin_) / (double) xbins_;

    return (count*xdel);
}

/**
 * Histogram1::read
 * @brief Read the histogram from an input file stream.
 */
core_inline
void Histogram1::read(atto::core::FileIn &fp)
{
    /* Set the input format of type double and size_t */
    std::string dfmt = atto::core::str_read_format<double>();
    std::string ufmt = atto::core::str_read_format<size_t>();

    /* Read the histogram bin number */
    size_t xbins;
    core_assert(fp.scanf(ufmt.c_str(), &xbins) == 1,
                 "failed to read histogram");
    core_assert(xbins > 1, "invalid histogram bins");

    xbins_ = xbins;

    /* Read the histogram x-ranges */
    double xmin, xmax;
    core_assert(fp.scanf(dfmt.c_str(), &xmin) == 1 &&
                 fp.scanf(dfmt.c_str(), &xmax) == 1,
                 "failed to read histogram");
    core_assert(xmin < xmax, "invalid histogram range");

    xmin_ = xmin;
    xmax_ = xmax;

    /*
     * Create a new pair of count and value vectors with
     * the new size and swap with the current ones.
     */
    Vector<double> count(xbins);
    count_ = std::move(count);

    Vector<double> value(xbins);
    value_ = std::move(value);

    /* Read the histogram data */
    for (size_t i = 0; i < xbins_; ++i) {
        double xval;
        core_assert(fp.scanf(dfmt.c_str(), &xval) == 1         &&
                     fp.scanf(dfmt.c_str(), &count_(i)) == 1    &&
                     fp.scanf(dfmt.c_str(), &value_(i)) == 1,
                     "failed to read histogram");
    }
}

/**
 * Histogram1::write
 * @brief Read the histogram to an output file stream.
 */
core_inline
void Histogram1::write(atto::core::FileOut &fp, bool normalize)
{
    /* Set the output format of type double and size_t */
    std::string dfmt = atto::core::str_write_format<double>();
    std::string ufmt = atto::core::str_write_format<size_t>();

    /* Histogram scale */
    double scale = normalize ? integral() : 1.0;
    if (scale > 0.0)
        scale = 1.0 / scale;

    double xdel = (xmax_ - xmin_) / (double) xbins_;

    /* Write the histogram bin number and range */
    core_assert(
        fp.printf(ufmt.c_str(), xbins_) > 0 &&
        fp.printf("\n") > 0                 &&
        fp.printf(dfmt.c_str(), xmin_) > 0  &&
        fp.printf("\n") > 0                 &&
        fp.printf(dfmt.c_str(), xmax_) > 0  &&
        fp.printf("\n") > 0,
        "failed to write histogram");

    for (size_t i = 0; i < xbins_; ++i) {
        double xval = xmin_ + xdel * (double) i;

        core_assert(
            fp.printf(dfmt.c_str(), xval) > 0               &&
            fp.printf(" ") > 0                              &&
            fp.printf(dfmt.c_str(), count_(i)) > 0          &&
            fp.printf(" ") > 0                              &&
            fp.printf(dfmt.c_str(), value_(i)*scale) > 0    &&
            fp.printf("\n") > 0,
            "failed to write histogram");
    }
}

} /* namespace algo */

#endif /* ALGO_NUMERIC_HISTOGRAM1_H */
