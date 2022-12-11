/*
 * array.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ALGO_NUMERIC_ARRAY_H_
#define ALGO_NUMERIC_ARRAY_H_

/** ---- Aligned array implementations ----------------------------------------
 */
#include "numeric/array/array1d.hpp"
#include "numeric/array/array2d.hpp"
#include "numeric/array/array3d.hpp"

namespace algo {

/** ---- Aligned array type aliases -------------------------------------------
 */
template<typename T>
using Vector = Array1d<T>;      /* vector typedef declaration as 1d-array. */

template<typename T>
using Matrix = Array2d<T>;      /* matrix typedef declaration as 2d-array. */

template<typename T>
using Tensor = Array3d<T>;      /* tensor typedef declaration as 3d-array. */

} /* namespace algo */

#endif /* ALGO_NUMERIC_ARRAY_H_ */
