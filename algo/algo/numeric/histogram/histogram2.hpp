/*
 * histogram2.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ALGO_NUMERIC_HISTOGRAM2_H_
#define ALGO_NUMERIC_HISTOGRAM2_H_

#include "atto/core/core.hpp"
#include "atto/math/math.hpp"
#include "numeric/array/array.hpp"

namespace algo {

/** ---- Histogram2 distribution ----------------------------------------------
 * struct Histogram2
 */
struct Histogram2 {
    /* Histogram member variables and access functions. */
    size_t xbins_ = 0;
    size_t ybins_ = 0;
    double xmin_ = 0.0;
    double xmax_ = 0.0;
    double ymin_ = 0.0;
    double ymax_ = 0.0;
    Matrix<double> count_;
    Matrix<double> value_;

    const double &count(size_t i, size_t j) const {
        core_assert(i < xbins_ && j < ybins_, "invalid histogram index");
        return count_(i,j);
    }

    const double &value(size_t i, size_t j) const {
        core_assert(i < xbins_ && j < ybins_, "invalid histogram index");
        return value_(i,j);
    }

    /* Constructor/destructor */
    Histogram2(size_t xbins, size_t ybins)
        : xbins_(xbins)
        , ybins_(ybins)
        , count_(Matrix<double>(xbins, ybins))
        , value_(Matrix<double>(xbins, ybins)) {}
    Histogram2(const size_t xbins,
            const size_t ybins,
            const double xmin,
            const double xmax,
            const double ymin,
            const double ymax)
        : xbins_(xbins)
        , ybins_(ybins)
        , xmin_(xmin)
        , xmax_(xmax)
        , ymin_(ymin)
        , ymax_(ymax)
        , count_(Matrix<double>(xbins, ybins))
        , value_(Matrix<double>(xbins, ybins)) {}
    ~Histogram2() {}

    /* Copy constructor/assignment. */
    Histogram2(const Histogram2 &other)
        : xbins_(other.xbins_)
        , ybins_(other.ybins_)
        , xmin_(other.xmin_)
        , xmax_(other.xmax_)
        , ymin_(other.ymin_)
        , ymax_(other.ymax_)
        , count_(other.count_)
        , value_(other.value_) {}

    Histogram2 &operator=(const Histogram2 &other) {
        if (this == &other) {
            return *this;
        }

        xbins_ = other.xbins_;
        ybins_ = other.ybins_;
        xmin_ = other.xmin_;
        xmax_ = other.xmax_;
        ymin_ = other.ymin_;
        ymax_ = other.ymax_;
        count_ = other.count_;
        value_ = other.value_;

        return *this;
    }

    /* Move constructor/assignment. */
    Histogram2(Histogram2 &&other)
        : xbins_(std::move(other.xbins_))
        , ybins_(std::move(other.ybins_))
        , xmin_(std::move(other.xmin_))
        , xmax_(std::move(other.xmax_))
        , ymin_(std::move(other.ymin_))
        , ymax_(std::move(other.ymax_))
        , count_(std::move(other.count_))
        , value_(std::move(other.value_)) {}

    Histogram2 &operator=(Histogram2 &&other) {
        if (this == &other) {
            return *this;
        }

        xbins_ = std::move(other.xbins_);
        ybins_ = std::move(other.ybins_);
        xmin_ = std::move(other.xmin_);
        xmax_ = std::move(other.xmax_);
        ymin_ = std::move(other.ymin_);
        ymax_ = std::move(other.ymax_);
        count_ = std::move(other.count_);
        value_ = std::move(other.value_);

        return *this;
    }

    /*
     * Histogram data functions.
     */
    size_t size(void) const { return xbins_; }
    void clear(void);

    void set(double xmin,
              double xmax,
              double ymin,
              double ymax,
              bool reset = false);
    void bin(double xval,
              double yval,
              double weight,
              bool midpoint = true);
    double integral(void) const;

    /*
     * Histogram input/output.
     */
    void read(atto::core::FileIn &fp);
    void write(atto::core::FileOut &fp, bool normalize = false);
};

/**
 * Histogram2::clear
 * @brief Reset the histogram count and value data.
 */
core_inline
void Histogram2::clear(void)
{
    for (size_t i = 0; i < xbins_; ++i) {
        for (size_t j = 0; j < ybins_; ++j) {
            count_(i,j) = 0.0;
            value_(i,j) = 0.0;
        }
    }
}

/**
 * Histogram2::set
 * @brief Set the histogram ranges and reset if needed.
 */
core_inline
void Histogram2::set(
    double xmin,
    double xmax,
    double ymin,
    double ymax,
    bool reset)
{
    core_assert(xmin < xmax && ymin < ymax, "invalid range");

    /* Set the histogram ranges */
    xmin_ = xmin;
    xmax_ = xmax;

    ymin_ = ymin;
    ymax_ = ymax;

    /* Reset the histogram */
    if (reset) {
        clear();
    }
}

/**
 * Histogram2::bin
 * @brief Bin a sample value with a given weight.
 */
core_inline
void Histogram2::bin(double xval, double yval, double weight, bool midpoint)
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
     *
     * and similarly for the y-direction.
     */
    double bias = midpoint ? 0.5 : 0.0;

    double ux = (double) xbins_ * (xval - xmin_) / (xmax_ - xmin_);
    size_t ix = std::floor(ux + bias);

    double uy = (double) ybins_ * (yval - ymin_) / (ymax_ - ymin_);
    size_t iy = std::floor(uy + bias);

    /* Increment the bin counter and bin value. */
    ix = atto::math::fclamp<size_t>(0, xbins_-1, ix);
    iy = atto::math::fclamp<size_t>(0, ybins_-1, iy);

    count_(ix, iy) += 1.0;
    value_(ix, iy) += weight;
}

/**
 * Histogram2::integral
 * @brief Compute the histogram probability integral.
 */
core_inline
double Histogram2::integral(void) const
{
    double count = 0.0;
    for (size_t i = 0; i < xbins_; ++i) {
        for (size_t j = 0; j < ybins_; ++j) {
            count += count_(i, j);
        }
    }

    double xdel = (xmax_ - xmin_) / (double) xbins_;
    double ydel = (ymax_ - ymin_) / (double) ybins_;

    return (count*xdel*ydel);
}

/**
 * Histogram2::read
 * @brief Read the histogram from an input file stream.
 */
core_inline
void Histogram2::read(atto::core::FileIn &fp)
{
    /* Set the input format of type double and size_t */
    std::string dfmt = atto::core::str_read_format<double>();
    std::string ufmt = atto::core::str_read_format<size_t>();

    /* Read the histogram bin number */
    size_t xbins, ybins;
    core_assert(fp.scanf(ufmt.c_str(), &xbins) == 1 &&
                 fp.scanf(ufmt.c_str(), &ybins) == 1,
                 "failed to read histogram");
    core_assert(xbins > 1 && ybins > 1, "invalid histogram bins");

    xbins_ = xbins;
    ybins_ = ybins;

    /* Read the histogram x- and y-ranges */
    double xmin, xmax, ymin, ymax;
    core_assert(fp.scanf(dfmt.c_str(), &xmin) == 1 &&
                 fp.scanf(dfmt.c_str(), &xmax) == 1 &&
                 fp.scanf(dfmt.c_str(), &ymin) == 1 &&
                 fp.scanf(dfmt.c_str(), &ymax) == 1,
                 "failed to read histogram");
    core_assert(xmin < xmax && ymin < ymax, "invalid histogram range");

    xmin_ = xmin;
    xmax_ = xmax;

    ymin_ = ymin;
    ymax_ = ymax;

    /*
     * Create a new pair of count and value vectors with
     * the new size and swap with the current ones.
     */
    Matrix<double> count(xbins, ybins);
    count_ = std::move(count);

    Matrix<double> value(xbins, ybins);
    value_ = std::move(value);

    /* Read the histogram data */
    for (size_t i = 0; i < xbins_; ++i) {
        for (size_t j = 0; j < ybins_; ++j) {
            double xval, yval;
            core_assert(fp.scanf(dfmt.c_str(), &xval) == 1         &&
                         fp.scanf(dfmt.c_str(), &yval) == 1         &&
                         fp.scanf(dfmt.c_str(), &count_(i,j)) == 1  &&
                         fp.scanf(dfmt.c_str(), &value_(i,j)) == 1,
                         "failed to read histogram");
        }
    }
}

/**
 * Histogram2::write
 * @brief Read the histogram to an output file stream.
 */
core_inline
void Histogram2::write(atto::core::FileOut &fp, bool normalize)
{
    /* Set the output format of type double and size_t */
    std::string dfmt = atto::core::str_write_format<double>();
    std::string ufmt = atto::core::str_write_format<size_t>();

    /* Histogram scale */
    double scale = normalize ? integral() : 1.0;
    if (scale > 0.0)
        scale = 1.0 / scale;

    double xdel = (xmax_ - xmin_) / (double) xbins_;
    double ydel = (ymax_ - ymin_) / (double) ybins_;

    /* Write the histogram bin number and range */
    core_assert(
        fp.printf(ufmt.c_str(), xbins_) > 0 &&
        fp.printf("\n") > 0                 &&
        fp.printf(ufmt.c_str(), ybins_) > 0 &&
        fp.printf("\n") > 0                 &&
        fp.printf(dfmt.c_str(), xmin_) > 0  &&
        fp.printf("\n") > 0                 &&
        fp.printf(dfmt.c_str(), xmax_) > 0  &&
        fp.printf("\n") > 0                 &&
        fp.printf(dfmt.c_str(), ymin_) > 0  &&
        fp.printf("\n") > 0                 &&
        fp.printf(dfmt.c_str(), ymax_) > 0  &&
        fp.printf("\n") > 0,
        "failed to write histogram");

    for (size_t i = 0; i < xbins_; ++i) {
        for (size_t j = 0; j < ybins_; ++j) {
            double xval = xmin_ + xdel * (double) i;
            double yval = ymin_ + ydel * (double) j;

            core_assert(
                fp.printf(dfmt.c_str(), xval) > 0               &&
                fp.printf(" ") > 0                              &&
                fp.printf(dfmt.c_str(), yval) > 0               &&
                fp.printf(" ") > 0                              &&
                fp.printf(dfmt.c_str(), count_(i,j)) > 0        &&
                fp.printf(" ") > 0                              &&
                fp.printf(dfmt.c_str(), value_(i,j)*scale) > 0  &&
                fp.printf("\n") > 0,
                "failed to write histogram");
        }
    }
}

} /* namespace algo */

#endif /* ALGO_NUMERIC_HISTOGRAM2_H_ */
