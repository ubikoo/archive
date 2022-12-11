/*
 * core.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef CORE_H_
#define CORE_H_

/**
 * Standard C headers
 *
 * #include <cstdlib>
 * #include <cstddef>
 * #include <cstdio>
 * #include <cstring>
 * #include <cstdint>
 * #include <cstdbool>
 * #include <cmath>
 * #include <cfloat>
 * #include <climits>
 * #include <cctype>
 * #include <ctime>
 * #include <cerrno>
 * #include <cassert>
 *
 * Standard C++ headers
 *
 * #include <iostream>
 * #include <fstream>
 * #include <sstream>
 * #include <iomanip>
 * #include <string>
 * #include <memory>
 *
 * #include <typeinfo>
 * #include <typeindex>
 * #include <type_traits>
 * #include <exception>
 * #include <chrono>
 *
 * Standard C++ containers
 *
 * #include <array>
 * #include <vector>
 * #include <list>
 * #include <deque>
 * #include <stack>
 * #include <queue>
 * #include <set>
 * #include <map>
 * #include <unordered_map>
 * #include <unordered_set>
 *
 * Standard C++ numerics
 *
 * #include <functional>
 * #include <algorithm>
 * #include <numeric>
 * #include <random>
 * #include <bitset>
 * #include <limits>
 *
 * OpenGL/GLFW headers using GLEW loader.
 * @note Define GLFW_INCLUDE_NONE before the GLFW header to explicitly
 * disable inclusion of the OpenGL environment header and use the header
 * provided by glad.
 * @see https://www.glfw.org/docs/3.3
 *
 * #define GLFW_INCLUDE_NONE
 * #include <glad/glad.h>
 * #include <GLFW/glfw3.h>
 *
 * OpenGL/GLFW headers using GLAD loader.
 * @note Make sure to include the GLEW header before the GLFW one.
 * The GLEW header defines macros that disable any OpenGL header
 * that the GLFW header includes and GLEW will work as expected.
 * @see https://www.glfw.org/docs/3.3
 *
 * #define GLEW_STATIC
 * #include <GL/glew.h>
 * #include <GLFW/glfw3.h>
 * #define GLFW_INCLUDE_NONE
 * #include <glad/glad.h>
 * #include <GLFW/glfw3.h>
 *
 * GLM linear algebra.
 *
 * #define GLM_FORCE_CXX11
 * #define GLM_FORCE_AVX
 * #define GLM_FORCE_INLINE
 * #include <glm/glm.hpp>
 * #include <glm/gtc/type_ptr.hpp>
 * #include <glm/gtc/matrix_transform.hpp>
 * #include <glm/gtx/transform.hpp>
 * #include <glm/gtx/rotate_vector.hpp>
 *
 * Eigen linear algebra.
 * @note Eigen dense header includes: Core, Geometry, LU, Cholesky,
 * SVD, QR, and Eigenvalues.
 * @see http://eigen.tuxfamily.org/dox
 *
 * #include <Eigen/Dense>
 *
 * OpenCV library.
 * @see https://docs.opencv.org/master/index.html
 *
 * #include <opencv2/core/core.hpp>
 * #include <opencv2/imgproc/imgproc.hpp>
 * #include <opencv2/highgui/highgui.hpp>
 */

/**
 * Core interface.
 */
#include "core/base.hpp"
#include "core/memory.hpp"
#include "core/string.hpp"
#include "core/file.hpp"

/**
 * Core math interface.
 */
#include "core/math.hpp"
#include "core/geometry.hpp"
#include "core/random.hpp"

/*
 * C linkage declarations
 */
#undef  __BEGIN_DECLS
#undef  __END_DECLS
#ifdef  __cplusplus
#define __BEGIN_DECLS extern "C" {
#define __END_DECLS }
#else
#define __BEGIN_DECLS   /* empty */
#define __END_DECLS     /* empty */
#endif
__BEGIN_DECLS
__END_DECLS

#endif /* CORE_H_ */
