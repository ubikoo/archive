/*
 * core.h
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
 * #include <stdlib.h>
 * #include <stddef.h>
 * #include <stdio.h>
 * #include <string.h>
 * #include <stdbool.h>
 * #include <stdint.h>
 * #include <math.h>
 * #include <float.h>
 * #include <limits.h>
 * #include <ctype.h>
 * #include <time.h>
 * #include <errno.h>
 * #include <assert.h>
 */

/**
 * OpenGL/GLFW headers using GLEW loader.
 * @note Define GLFW_INCLUDE_NONE before the GLFW header to explicitly
 * disable inclusion of the OpenGL environment header and use the header
 * provided by glad.
 * @see https://www.glfw.org/docs/3.3
 *
 * #define GLFW_INCLUDE_NONE
 * #include <glad/glad.h>
 * #include <GLFW/glfw3.h>
 */

/**
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
 */

/**
 * Linmath linear algebra.
 *
 * #include <linmath/linmath.h>
 */

/**
 * Core interface.
 */
#include "core/base.h"
#include "core/memory.h"
#include "core/array.h"
#include "core/string.h"
#include "core/file.h"

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
