/*
 * test-vec4.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_CORE_GEOMETRY_VEC4_H_
#define TEST_CORE_GEOMETRY_VEC4_H_

/** ---------------------------------------------------------------------------
 * test_vec4<Type>
 * @brief vec4<Type> test client.
 */
template<typename Type>
void test_vec4 (void)
{
    {   /* test default constructor */
        core::math::vec4<Type> a;
        core::math::vec4<Type> b(a);
        core::math::vec4<Type> c;
        core::math::vec4<Type> d = c = b;
        for (size_t i = 0; i < a.length(); ++i) {
            REQUIRE(b(i) == a(i));
            REQUIRE(c(i) == a(i));
            REQUIRE(d(i) == a(i));
        }
    }

    {   /* test element constructor */
        core::math::vec4<Type> a(1,2,3,4);
        core::math::vec4<Type> b(a);
        core::math::vec4<Type> c;
        core::math::vec4<Type> d = c = b;
        for (size_t i = 0; i < a.length(); ++i) {
            REQUIRE(b(i) == static_cast<Type>(i+1));
            REQUIRE(c(i) == static_cast<Type>(i+1));
            REQUIRE(d(i) == static_cast<Type>(i+1));
        }
    }

    {   /* test data constructor */
        Type data[4] = {1,2,3,4};
        core::math::vec4<Type> a(&data[0]);
        core::math::vec4<Type> b(a);
        core::math::vec4<Type> c;
        core::math::vec4<Type> d = c = b;
        for (size_t i = 0; i < a.length(); ++i) {
            REQUIRE(b(i) == static_cast<Type>(i+1));
            REQUIRE(c(i) == static_cast<Type>(i+1));
            REQUIRE(d(i) == static_cast<Type>(i+1));
        }
    }

    {   /* test unary scalar operators */
        const Type one = static_cast<Type>(1);
        const Type two = static_cast<Type>(2);
        const Type three = static_cast<Type>(3);
        const Type six = static_cast<Type>(6);

        core::math::vec4<Type> a(1,2,3,4);
        a += two;     /* a=(3,4,5,6) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(a(i) == static_cast<Type>(i+1+2));
        }

        a -= one;     /* a=(2,3,4,5) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(a(i) == static_cast<Type>(i+1+1));
        }

        a *= six;     /* a=(12,18,24,30) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(a(i) == static_cast<Type>((i+1+1)*6));
        }

        a /= three;     /* a=(4,6,8,10) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(a(i) == static_cast<Type>((i+1+1)*2));
        }
    }

    {   /* test unary vector operators */
        core::math::vec4<Type> one(1);
        core::math::vec4<Type> two = one + one;                      /* + */
        core::math::vec4<Type> three = two + two - one;              /* +,- */
        core::math::vec4<Type> six = ((three + one) * three) / two;  /* +,*,/ */

        core::math::vec4<Type> a(1,2,3,4);
        a += two;     /* a=(3,4,5,6) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(a(i) == static_cast<Type>(i+1+2));
        }

        a -= one;     /* a=(2,3,4,5) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(a(i) == static_cast<Type>(i+1+1));
        }

        a *= six;     /* a=(12,18,24,30) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(a(i) == static_cast<Type>((i+1+1)*6));
        }

        a /= three;     /* a=(4,6,8,10) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(a(i) == static_cast<Type>((i+1+1)*2));
        }
    }

    {   /* test increment operators */
        core::math::vec4<Type> one(1);
        core::math::vec4<Type> two(one);
        two++;
        two--;
        ++two;
        ++two;
        core::math::vec4<Type> three = two--;
        core::math::vec4<Type> six = ((three + one) * three) / two;  /* +,*,/ */

        core::math::vec4<Type> a(1,2,3,4);
        a += two;     /* a=(3,4,5,6) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(a(i) == static_cast<Type>(i+1+2));
        }

        a -= one;     /* a=(2,3,4,5) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(a(i) == static_cast<Type>(i+1+1));
        }

        a *= six;     /* a=(12,18,24,30) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(a(i) == static_cast<Type>((i+1+1)*6));
        }

        a /= three;     /* a=(4,6,8,10) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(a(i) == static_cast<Type>((i+1+1)*2));
        }
    }
}


/** ---------------------------------------------------------------------------
 * test_vec4<float>
 * @brief vec4<float> test client.
 */
template<>
void test_vec4<float> (void)
{
    {   /* test default constructor */
        core::math::vec4<float> a;
        core::math::vec4<float> b(a);
        core::math::vec4<float> c;
        core::math::vec4<float> d = c = b;
        for (size_t i = 0; i < a.length(); ++i) {
            REQUIRE(core::math::isequal(b(i), a(i)));
            REQUIRE(core::math::isequal(c(i), a(i)));
            REQUIRE(core::math::isequal(d(i), a(i)));
        }
    }

    {   /* test element constructor */
        core::math::vec4<float> a(1,2,3,4);
        core::math::vec4<float> b(a);
        core::math::vec4<float> c;
        core::math::vec4<float> d = c = b;
        for (size_t i = 0; i < a.length(); ++i) {
            REQUIRE(core::math::isequal(b(i), static_cast<float>(i+1)));
            REQUIRE(core::math::isequal(c(i), static_cast<float>(i+1)));
            REQUIRE(core::math::isequal(d(i), static_cast<float>(i+1)));
        }
    }

    {   /* test data constructor */
        float data[4] = {1,2,3,4};
        core::math::vec4<float> a(&data[0]);
        core::math::vec4<float> b(a);
        core::math::vec4<float> c;
        core::math::vec4<float> d = c = b;
        for (size_t i = 0; i < a.length(); ++i) {
            REQUIRE(core::math::isequal(b(i), static_cast<float>(i+1)));
            REQUIRE(core::math::isequal(c(i), static_cast<float>(i+1)));
            REQUIRE(core::math::isequal(d(i), static_cast<float>(i+1)));
        }
    }

    {   /* test unary scalar operators */
        const float one = static_cast<float>(1);
        const float two = static_cast<float>(2);
        const float three = static_cast<float>(3);
        const float six = static_cast<float>(6);

        core::math::vec4<float> a(1,2,3,4);
        a += two;     /* a=(3,4,5,6) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<float>(i+1+2)));
        }

        a -= one;     /* a=(2,3,4,5) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<float>(i+1+1)));
        }

        a *= six;     /* a=(12,18,24,30) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<float>((i+1+1)*6)));
        }

        a /= three;     /* a=(4,6,8,10) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<float>((i+1+1)*2)));
        }
    }

    {   /* test unary vector operators */
        core::math::vec4<float> one(1);
        core::math::vec4<float> two = one + one;                      /* + */
        core::math::vec4<float> three = two + two - one;              /* +,- */
        core::math::vec4<float> six = ((three + one) * three) / two;  /* +,*,/ */

        core::math::vec4<float> a(1,2,3,4);
        a += two;     /* a=(3,4,5,6) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<float>(i+1+2)));
        }

        a -= one;     /* a=(2,3,4,5) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<float>(i+1+1)));
        }

        a *= six;     /* a=(12,18,24,30) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<float>((i+1+1)*6)));
        }

        a /= three;     /* a=(4,6,8,10) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<float>((i+1+1)*2)));
        }
    }

    {   /* test increment operators */
        core::math::vec4<float> one(1);
        core::math::vec4<float> two(one);
        two++;
        two--;
        ++two;
        ++two;
        core::math::vec4<float> three = two--;
        core::math::vec4<float> six = ((three + one) * three) / two;  /* +,*,/ */

        core::math::vec4<float> a(1,2,3,4);
        a += two;     /* a=(3,4,5,6) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<float>(i+1+2)));
        }

        a -= one;     /* a=(2,3,4,5) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<float>(i+1+1)));
        }

        a *= six;     /* a=(12,18,24,30) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<float>((i+1+1)*6)));
        }

        a /= three;     /* a=(4,6,8,10) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<float>((i+1+1)*2)));
        }
    }
}


/** ---------------------------------------------------------------------------
 * test_vec4<double>
 * @brief vec4<double> test client.
 */
template<>
void test_vec4<double> (void)
{
    {   /* test default constructor */
        core::math::vec4<double> a;
        core::math::vec4<double> b(a);
        core::math::vec4<double> c;
        core::math::vec4<double> d = c = b;
        for (size_t i = 0; i < a.length(); ++i) {
            REQUIRE(core::math::isequal(b(i), a(i)));
            REQUIRE(core::math::isequal(c(i), a(i)));
            REQUIRE(core::math::isequal(d(i), a(i)));
        }
    }

    {   /* test element constructor */
        core::math::vec4<double> a(1,2,3,4);
        core::math::vec4<double> b(a);
        core::math::vec4<double> c;
        core::math::vec4<double> d = c = b;
        for (size_t i = 0; i < a.length(); ++i) {
            REQUIRE(core::math::isequal(b(i), static_cast<double>(i+1)));
            REQUIRE(core::math::isequal(c(i), static_cast<double>(i+1)));
            REQUIRE(core::math::isequal(d(i), static_cast<double>(i+1)));
        }
    }

    {   /* test data constructor */
        double data[4] = {1,2,3,4};
        core::math::vec4<double> a(&data[0]);
        core::math::vec4<double> b(a);
        core::math::vec4<double> c;
        core::math::vec4<double> d = c = b;
        for (size_t i = 0; i < a.length(); ++i) {
            REQUIRE(core::math::isequal(b(i), static_cast<double>(i+1)));
            REQUIRE(core::math::isequal(c(i), static_cast<double>(i+1)));
            REQUIRE(core::math::isequal(d(i), static_cast<double>(i+1)));
        }
    }

    {   /* test unary scalar operators */
        const double one = static_cast<double>(1);
        const double two = static_cast<double>(2);
        const double three = static_cast<double>(3);
        const double six = static_cast<double>(6);

        core::math::vec4<double> a(1,2,3,4);
        a += two;     /* a=(3,4,5,6) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<double>(i+1+2)));
        }

        a -= one;     /* a=(2,3,4,5) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<double>(i+1+1)));
        }

        a *= six;     /* a=(12,18,24,30) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<double>((i+1+1)*6)));
        }

        a /= three;     /* a=(4,6,8,10) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<double>((i+1+1)*2)));
        }
    }

    {   /* test unary vector operators */
        core::math::vec4<double> one(1);
        core::math::vec4<double> two = one + one;                      /* + */
        core::math::vec4<double> three = two + two - one;              /* +,- */
        core::math::vec4<double> six = ((three + one) * three) / two;  /* +,*,/ */

        core::math::vec4<double> a(1,2,3,4);
        a += two;     /* a=(3,4,5,6) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<double>(i+1+2)));
        }

        a -= one;     /* a=(2,3,4,5) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<double>(i+1+1)));
        }

        a *= six;     /* a=(12,18,24,30) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<double>((i+1+1)*6)));
        }

        a /= three;     /* a=(4,6,8,10) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<double>((i+1+1)*2)));
        }
    }

    {   /* test increment operators */
        core::math::vec4<double> one(1);
        core::math::vec4<double> two(one);
        two++;
        two--;
        ++two;
        ++two;
        core::math::vec4<double> three = two--;
        core::math::vec4<double> six = ((three + one) * three) / two;  /* +,*,/ */

        core::math::vec4<double> a(1,2,3,4);
        a += two;     /* a=(3,4,5,6) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<double>(i+1+2)));
        }

        a -= one;     /* a=(2,3,4,5) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<double>(i+1+1)));
        }

        a *= six;     /* a=(12,18,24,30) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<double>((i+1+1)*6)));
        }

        a /= three;     /* a=(4,6,8,10) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<double>((i+1+1)*2)));
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

#endif /* TEST_CORE_GEOMETRY_VEC4_H_ */