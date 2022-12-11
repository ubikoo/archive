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
static const int32_t n_sites = 1024;
static const double p_freq = 0.1;
static const double p_lo = 0.5;
static const double p_hi = 0.7;

/* OpenGL parameters */
static const int window_width = 1024;
static const int window_height = 1024;
static const char window_title[] = "Empty";
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
