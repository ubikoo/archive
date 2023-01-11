/*
 * test-arithmetic4.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_CORE_GEOMETRY_ARITHMETIC4_H_
#define TEST_CORE_GEOMETRY_ARITHMETIC4_H_

/** ---------------------------------------------------------------------------
 * test_arithmetic4<Type>
 * @brief vec4<Type> arithmetic test client.
 */
template<typename Type>
void test_arithmetic4 (void)
{
    { /* test abs */
        core::math::vec4<Type> a, abs_a;

        a = core::math::vec4<Type>(-3,-2,-1,0);
        abs_a = abs(a);
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(abs_a(i), std::fabs(a(i))));
        }

        a = core::math::vec4<Type>(-2,-1,1,2);
        abs_a = abs(a);
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(abs_a(i), std::fabs(a(i))));
        }
    }

    { /* test sign */
        core::math::vec4<Type> a, sign_a;

        a = core::math::vec4<Type>(-2,-3,-4,-5);
        sign_a = sign(a);
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(sign_a(i), static_cast<Type>(-1)));
        }

        a = core::math::vec4<Type>(2,3,4,5);
        sign_a = sign(a);
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(sign_a(i), static_cast<Type>(1)));
        }

        a = core::math::vec4<Type>(0,0,0,0);
        sign_a = sign(a);
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(sign_a(i), static_cast<Type>(0)));
        }
    }

    { /* test floor */
        core::math::vec4<Type> a, floor_a;

        a = core::math::vec4<Type>(-2.1, -1.2, -0.8, -0.4);
        floor_a = floor(a);
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(floor_a(i), std::floor(a(i))));
        }

        a = core::math::vec4<Type>(0.8, 0.4, 1.2, 2.1);
        floor_a = floor(a);
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(floor_a(i), std::floor(a(i))));
        }
    }

    { /* test round */
        core::math::vec4<Type> a, round_a;

        a = core::math::vec4<Type>(-2.3, -2.6, -1.3, -1.6);
        round_a = round(a);
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(round_a(i), std::round(a(i))));
        }

        a = core::math::vec4<Type>(1.3, 1.6, 2.3, 2.6);
        round_a = round(a);
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(round_a(i), std::round(a(i))));
        }
    }

    { /* test ceil */
        core::math::vec4<Type> a, ceil_a;

        a = core::math::vec4<Type>(-2.3, -2.6, -1.3, -1.6);
        ceil_a = ceil(a);
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(ceil_a(i), std::ceil(a(i))));
        }

        a = core::math::vec4<Type>(1.3, 1.6, 2.3, 2.6);
        ceil_a = ceil(a);
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(ceil_a(i), std::ceil(a(i))));
        }
    }

    { /* test clamp */
        auto clamp_fun = [&] (
            const Type val,
            const Type lo,
            const Type hi) -> Type {
            return std::min(std::max(val,lo),hi);
        };

        core::math::vec4<Type> a, clamp_a;
        Type lo = -1.0;
        Type hi =  0.0;
        a = core::math::vec4<Type>(-1.5, -1.0, -0.5, 0.0);
        clamp_a = clamp(a, lo, hi);
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(clamp_a(i), clamp_fun(a(i), lo, hi)));
        }

        lo =  0.0;
        hi =  1.0;
        a = core::math::vec4<Type>(0.0, 0.5, 1.0, 1.5);
        clamp_a = clamp(a, lo, hi);
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(clamp_a(i), clamp_fun(a(i), lo, hi)));
        }
    }

    { /* test lerp */
        const size_t numsteps = 10000;
        constexpr Type one = static_cast<Type>(1);
        constexpr Type zero = static_cast<Type>(0);
        constexpr Type delta = 1.0 / (Type) numsteps;

        auto lerp_fun = [&] (
            const core::math::vec4<Type> &lo,
            const core::math::vec4<Type> &hi,
            const Type alpha) -> core::math::vec4<Type> {
            return (lo*(one - alpha) + hi*alpha);
        };

        core::math::vec4<Type> lo(-1.0);
        core::math::vec4<Type> hi( 0.0);
        Type alpha = 0.0;
        while (alpha < 1.0) {
            core::math::vec4<Type> a = lerp(lo,  hi, alpha);
            core::math::vec4<Type> b = lerp_fun(lo,  hi, alpha);
            core::math::vec4<Type> e = (a - b);
            for (size_t i = 0; i < a.length(); ++i) {
                REQUIRE(core::math::isequal(std::fabs(e(i)), zero));
            }
            alpha += delta;
        }

        lo = core::math::vec4<Type>(0.0);
        hi = core::math::vec4<Type>(1.0);
        alpha = 0.0;
        while (alpha < 1.0) {
            core::math::vec4<Type> a = lerp(lo,  hi, alpha);
            core::math::vec4<Type> b = lerp_fun(lo,  hi, alpha);
            core::math::vec4<Type> e = (a - b);
            for (size_t i = 0; i < a.length(); ++i) {
                REQUIRE(core::math::isequal(std::fabs(e(i)), zero));
            }
            alpha += delta;
        }
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

#endif /* TEST_CORE_GEOMETRY_ARITHMETIC4_H_ */
