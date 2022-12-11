/*
 * poincare.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "poincare.hpp"
using namespace atto;

/**
 * Poincare::intersect
 * @brief Compute the plane intersection with a ray given by two points.
 */
bool Poincare::intersect(
    const atto::math::vec3d &x_0,
    const atto::math::vec3d &x_1,
    atto::math::vec3d &x_isect)
{
    /* Compute intersection point on the Poincare plane. */
    atto::math::vec3d dir = atto::math::normalize(x_1 - x_0);
    double dot_0 = atto::math::dot(x_0, normal);
    double dot_1 = atto::math::dot(dir, normal);

    /* If x_isect is between x_0 and x_1, then the intersection is valid. */
    double t = (atto::math::fabs(dot_1) > 0.0) ? -dot_0 / dot_1 : 0.0;
    if (atto::math::fabs(t) < atto::math::norm(x_1 - x_0)) {
        x_isect = x_0 + t * dir;
        return true;
    }
    return false;
}

/**
 * Poincare::intersect
 * @brief Compute the plane intersection with a ray given by two points.
 */
std::vector<atto::math::vec3d> Poincare::generate(
    const uint32_t n_points,
    const double x_lo,
    const double x_hi,
    const double y_lo,
    const double y_hi)
{
    math::Orthod ortho = math::Orthod::create_from_w(normal);

    double x_delta = Params::scale*(x_hi - x_lo) / static_cast<double>(n_points);
    double y_delta = Params::scale*(y_hi - y_lo) / static_cast<double>(n_points);

    std::vector<atto::math::vec3d> points;
    for (size_t i = 0; i < n_points; ++i) {
        for (size_t j = 0; j < n_points; ++j) {
            double x = Params::scale*x_lo + i * x_delta;
            double y = Params::scale*y_lo + j * y_delta;
            atto::math::vec3d pt = x * ortho.u + y * ortho.v;
            points.push_back(pt);
        }
    }
    return points;

}
