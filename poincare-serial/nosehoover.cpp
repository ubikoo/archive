/*
 * nosehoover.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "nosehoover.hpp"
using namespace atto;

/**
 * NoseHoover::deriv
 * @brief Compute Nose-Hoover time derivative.
 */
atto::math::vec3d NoseHoover::deriv(atto::math::vec3d &x)
{
    return atto::math::vec3d{
        x(1) / mass,
        -kappa * x(0) - x(2) * x(1),
        (x(1) * x(1) / (mass * temp) - 1.0) / tau};
}

/**
 * NoseHoover::step
 * @brief Compute Nose-Hoover integration step.
 */
void NoseHoover::step(double t_step, double max_err, uint64_t max_iter)
{
    double err = std::numeric_limits<double>::max();
    uint64_t n_iter = 0;
    atto::math::vec3d x_new = x_state;
    while (err > max_err && n_iter < max_iter) {
        atto::math::vec3d x_mid = 0.5 * (x_state + x_new);
        atto::math::vec3d dx_dt = deriv(x_mid);

        atto::math::vec3d x_old = x_new;
        x_new = x_state + t_step * dx_dt;
        err = atto::math::norm(x_new - x_old);

        ++n_iter;
    }
    x_state = x_new;
}

