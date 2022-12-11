/*
 * poincare.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef POINCARE_H_
#define POINCARE_H_

#include <vector>
#include "base.hpp"

/**
 * @brief Poincare represents a intersection plane centred at the origin
 * (0,0,0) and with a given normal.
 */
struct Poincare {
    atto::math::vec3d normal;

    /** Compute the plane intersection with a ray given by two points. */
    bool intersect(
        const atto::math::vec3d &x_0,
        const atto::math::vec3d &x_1,
        atto::math::vec3d &x_isect);

    /** Generate a collection of points within the specified range. */
    std::vector<atto::math::vec3d> generate(
        const uint32_t n_points,
        const double x_lo,
        const double x_hi,
        const double y_lo,
        const double y_hi);
};

#endif /* POINCARE_H_ */
