/*
 * base.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef BASE_H_
#define BASE_H_

#include "atto/opencl/opencl.hpp"

namespace Params {
/* Model parameters */
static const uint64_t n_steps = 10000;  /* Integration steps */
static const double t_step = 0.01;      /* Integration time step size */
static const double max_err = 1.0E-5;   /* Midpoint max error */
static const uint64_t max_iter = 3;     /* Midpoing max iterations */

static const double mass = 1.0;         /* Nose-Hoover mass */
static const double kappa = 1.0;        /* Nose-Hoover orce constant */
static const double tau = 10.0;          /* Nose-Hoover thermostat constant */
static const double temp = 0.1;         /* Nose-Hoover thermostat temperature */

static const uint32_t n_points = 1024;
static const double q_lo = -3.0;        /* thermostat position lower bound */
static const double q_hi =  3.0;        /* thermostat position upper bound */
static const double p_lo = -3.0;        /* thermostat momentum lower bound */
static const double p_hi =  3.0;        /* thermostat momentum upper bound */
static const double scale = 0.1;

/* OpenGL parameters */
static const int window_width = 1024;
static const int window_height = 1024;
static const char window_title[] = "Poincare";
static const double poll_timeout = 0.01;

/* OpenCL parameters */
// static const cl_ulong n_points = 1048576;
// static const cl_ulong device_index = 2;
// static const cl_ulong work_group_size = 256;
// static const cl_ulong num_work_items = atto::cl::NDRange::Roundup(
//     n_points, work_group_size);
// static const cl_ulong num_work_groups = num_work_items / work_group_size;
} /* Params */

#endif /* BASE_H_ */
