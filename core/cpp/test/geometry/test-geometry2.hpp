/*
 * test-geometry2.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_CORE_GEOMETRY_METRY2_H_
#define TEST_CORE_GEOMETRY_METRY2_H_

/** ---------------------------------------------------------------------------
 * test_geometry2<Type>
 * @brief vec2<Type> arithmetic test client.
 */
template<typename Type>
void test_geometry2 (void)
{
    { /* test dot */
        const Type zero = (Type) 0.0;
        const Type one = (Type) 1.0;
        const Type two = (Type) 2.0;

        core::math::vec2<Type> a( 1.0, 1.0);
        core::math::vec2<Type> b( 0.0, 1.0);
        core::math::vec2<Type> c(-1.0, 1.0);

        REQUIRE(core::math::isequal(dot(a,a), two));
        REQUIRE(core::math::isequal(dot(b,b), one));
        REQUIRE(core::math::isequal(dot(c,c), two));

        REQUIRE(core::math::isequal(dot(a,b), one));
        REQUIRE(core::math::isequal(dot(a,c), zero));
        REQUIRE(core::math::isequal(dot(b,c), one));
    }

    { /* test norm */
        const Type sqrt2 = (Type) std::sqrt(2.0);
        const Type one = (Type) 1.0;

        core::math::vec2<Type> a( 1.0, 1.0);
        core::math::vec2<Type> b( 0.0, 1.0);
        core::math::vec2<Type> c(-1.0, 1.0);

        REQUIRE(core::math::isequal(norm(a), sqrt2));
        REQUIRE(core::math::isequal(norm(b), one));
        REQUIRE(core::math::isequal(norm(c), sqrt2));
    }

    { /* test normalize */
        const Type one = (Type) 1.0;

        core::math::vec2<Type> a( 1.0, 1.0);
        core::math::vec2<Type> b( 0.0, 1.0);
        core::math::vec2<Type> c(-1.0, 1.0);

        core::math::vec2<Type> norm_a = normalize(a);
        core::math::vec2<Type> norm_b = normalize(b);
        core::math::vec2<Type> norm_c = normalize(c);

        REQUIRE(core::math::isequal(norm(norm_a), one));
        REQUIRE(core::math::isequal(norm(norm_b), one));
        REQUIRE(core::math::isequal(norm(norm_c), one));
    }

    { /* test distance */
        const Type zero = (Type) 0.0;

        core::math::vec2<Type> a( 1.0, 1.0);
        core::math::vec2<Type> b( 0.0, 1.0);
        core::math::vec2<Type> c(-1.0, 1.0);

        core::math::vec2<Type> ab  = a - b;
        core::math::vec2<Type> ac  = a - c;
        core::math::vec2<Type> cb1 = c - b;
        core::math::vec2<Type> cb2 = ab - ac;

        REQUIRE(core::math::isequal(norm(ab), distance(a,b)));
        REQUIRE(core::math::isequal(distance(b,a), distance(a,b)));

        REQUIRE(core::math::isequal(norm(ac), distance(a,c)));
        REQUIRE(core::math::isequal(distance(a,c), distance(a,c)));

        REQUIRE(core::math::isequal(norm(cb1), norm(cb2)));
        REQUIRE(core::math::isequal(norm(cb1), distance(c,b)));
        REQUIRE(core::math::isequal(distance(b,c), distance(c,b)));
        REQUIRE(core::math::isequal(distance(cb1,cb2), zero));
    }
}

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

#endif /* TEST_CORE_GEOMETRY_METRY2_H_ */
