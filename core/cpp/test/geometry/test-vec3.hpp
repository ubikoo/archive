/*
 * test-vec3.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_CORE_GEOMETRY_VEC3_H_
#define TEST_CORE_GEOMETRY_VEC3_H_

/** ---------------------------------------------------------------------------
 * test_vec3<Type>
 * @brief vec3<Type> test client.
 */
template<typename Type>
void test_vec3 (void)
{
    {   /* test default constructor */
        core::math::vec3<Type> a;
        core::math::vec3<Type> b(a);
        core::math::vec3<Type> c;
        core::math::vec3<Type> d = c = b;
        for (size_t i = 0; i < a.length(); ++i) {
            REQUIRE(b(i) == a(i));
            REQUIRE(c(i) == a(i));
            REQUIRE(d(i) == a(i));
        }
    }

    {   /* test element constructor */
        core::math::vec3<Type> a(1,2,3);
        core::math::vec3<Type> b(a);
        core::math::vec3<Type> c;
        core::math::vec3<Type> d = c = b;
        for (size_t i = 0; i < a.length(); ++i) {
            REQUIRE(b(i) == static_cast<Type>(i+1));
            REQUIRE(c(i) == static_cast<Type>(i+1));
            REQUIRE(d(i) == static_cast<Type>(i+1));
        }
    }

    {   /* test data constructor */
        Type data[3] = {1,2,3};
        core::math::vec3<Type> a(&data[0]);
        core::math::vec3<Type> b(a);
        core::math::vec3<Type> c;
        core::math::vec3<Type> d = c = b;
        for (size_t i = 0; i < a.length(); ++i) {
            REQUIRE(b(i) == static_cast<Type>(i+1));
            REQUIRE(c(i) == static_cast<Type>(i+1));
            REQUIRE(d(i) == static_cast<Type>(i+1));
        }
    }

    {   /* test unary scalar operators */
        core::math::vec3<Type> a(1,2,3);
        a += 2;     /* a=(3,4,5) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(a(i) == static_cast<Type>(i+1+2));
        }

        a -= 1;     /* a=(2,3,4) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(a(i) == static_cast<Type>(i+1+1));
        }

        a *= 6;     /* a=(12,18,24) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(a(i) == static_cast<Type>((i+1+1)*6));
        }

        a /= 3;     /* a=(4,6,8) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(a(i) == static_cast<Type>((i+1+1)*2));
        }
    }

    {   /* test unary vector operators */
        core::math::vec3<Type> one(1);
        core::math::vec3<Type> two = one + one;                      /* + */
        core::math::vec3<Type> three = two + two - one;              /* +,- */
        core::math::vec3<Type> six = ((three + one) * three) / two;  /* +,*,/ */

        core::math::vec3<Type> a(1,2,3);
        a += two;     /* a=(3,4,5) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(a(i) == static_cast<Type>(i+1+2));
        }

        a -= one;     /* a=(2,3,4) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(a(i) == static_cast<Type>(i+1+1));
        }

        a *= six;     /* a=(12,18,24) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(a(i) == static_cast<Type>((i+1+1)*6));
        }

        a /= three;     /* a=(4,6,8) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(a(i) == static_cast<Type>((i+1+1)*2));
        }
    }

    {   /* test increment operators */
        core::math::vec3<Type> one(1);
        core::math::vec3<Type> two(one);
        two++;
        two--;
        ++two;
        ++two;
        core::math::vec3<Type> three = two--;
        core::math::vec3<Type> six = ((three + one) * three) / two;  /* +,*,/ */

        core::math::vec3<Type> a(1,2,3);
        a += two;     /* a=(3,4,5) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(a(i) == static_cast<Type>(i+1+2));
        }

        a -= one;     /* a=(2,3,4) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(a(i) == static_cast<Type>(i+1+1));
        }

        a *= six;     /* a=(12,18,24) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(a(i) == static_cast<Type>((i+1+1)*6));
        }

        a /= three;     /* a=(4,6,8) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(a(i) == static_cast<Type>((i+1+1)*2));
        }
    }
}


/** ---------------------------------------------------------------------------
 * test_vec3<float>
 * @brief vec3<float> test client.
 */
template<>
void test_vec3<float> (void)
{
    {   /* test default constructor */
        core::math::vec3<float> a;
        core::math::vec3<float> b(a);
        core::math::vec3<float> c;
        core::math::vec3<float> d = c = b;
        for (size_t i = 0; i < a.length(); ++i) {
            REQUIRE(core::math::isequal(b(i), a(i)));
            REQUIRE(core::math::isequal(c(i), a(i)));
            REQUIRE(core::math::isequal(d(i), a(i)));
        }
    }

    {   /* test element constructor */
        core::math::vec3<float> a(1,2,3);
        core::math::vec3<float> b(a);
        core::math::vec3<float> c;
        core::math::vec3<float> d = c = b;
        for (size_t i = 0; i < a.length(); ++i) {
            REQUIRE(core::math::isequal(b(i), static_cast<float>(i+1)));
            REQUIRE(core::math::isequal(c(i), static_cast<float>(i+1)));
            REQUIRE(core::math::isequal(d(i), static_cast<float>(i+1)));
        }
    }

    {   /* test data constructor */
        float data[3] = {1,2,3};
        core::math::vec3<float> a(&data[0]);
        core::math::vec3<float> b(a);
        core::math::vec3<float> c;
        core::math::vec3<float> d = c = b;
        for (size_t i = 0; i < a.length(); ++i) {
            REQUIRE(core::math::isequal(b(i), static_cast<float>(i+1)));
            REQUIRE(core::math::isequal(c(i), static_cast<float>(i+1)));
            REQUIRE(core::math::isequal(d(i), static_cast<float>(i+1)));
        }
    }

    {   /* test unary scalar operators */
        core::math::vec3<float> a(1,2,3);
        a += 2;     /* a=(3,4,5) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<float>(i+1+2)));
        }

        a -= 1;     /* a=(2,3,4) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<float>(i+1+1)));
        }

        a *= 6;     /* a=(12,18,24) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<float>((i+1+1)*6)));
        }

        a /= 3;     /* a=(4,6,8) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<float>((i+1+1)*2)));
        }
    }

    {   /* test unary vector operators */
        core::math::vec3<float> one(1);
        core::math::vec3<float> two = one + one;                      /* + */
        core::math::vec3<float> three = two + two - one;              /* +,- */
        core::math::vec3<float> six = ((three + one) * three) / two;  /* +,*,/ */

        core::math::vec3<float> a(1,2,3);
        a += two;     /* a=(3,4,5) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<float>(i+1+2)));
        }

        a -= one;     /* a=(2,3,4) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<float>(i+1+1)));
        }

        a *= six;     /* a=(12,18,24) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<float>((i+1+1)*6)));
        }

        a /= three;     /* a=(4,6,8) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<float>((i+1+1)*2)));
        }
    }

    {   /* test increment operators */
        core::math::vec3<float> one(1);
        core::math::vec3<float> two(one);
        two++;
        two--;
        ++two;
        ++two;
        core::math::vec3<float> three = two--;
        core::math::vec3<float> six = ((three + one) * three) / two;  /* +,*,/ */

        core::math::vec3<float> a(1,2,3);
        a += two;     /* a=(3,4,5) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<float>(i+1+2)));
        }

        a -= one;     /* a=(2,3,4) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<float>(i+1+1)));
        }

        a *= six;     /* a=(12,18,24) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<float>((i+1+1)*6)));
        }

        a /= three;     /* a=(4,6,8) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<float>((i+1+1)*2)));
        }
    }
}


/** ---------------------------------------------------------------------------
 * test_vec3<double>
 * @brief vec3<double> test client.
 */
template<>
void test_vec3<double> (void)
{
    {   /* test default constructor */
        core::math::vec3<double> a;
        core::math::vec3<double> b(a);
        core::math::vec3<double> c;
        core::math::vec3<double> d = c = b;
        for (size_t i = 0; i < a.length(); ++i) {
            REQUIRE(core::math::isequal(b(i), a(i)));
            REQUIRE(core::math::isequal(c(i), a(i)));
            REQUIRE(core::math::isequal(d(i), a(i)));
        }
    }

    {   /* test element constructor */
        core::math::vec3<double> a(1,2,3);
        core::math::vec3<double> b(a);
        core::math::vec3<double> c;
        core::math::vec3<double> d = c = b;
        for (size_t i = 0; i < a.length(); ++i) {
            REQUIRE(core::math::isequal(b(i), static_cast<double>(i+1)));
            REQUIRE(core::math::isequal(c(i), static_cast<double>(i+1)));
            REQUIRE(core::math::isequal(d(i), static_cast<double>(i+1)));
        }
    }

    {   /* test data constructor */
        double data[3] = {1,2,3};
        core::math::vec3<double> a(&data[0]);
        core::math::vec3<double> b(a);
        core::math::vec3<double> c;
        core::math::vec3<double> d = c = b;
        for (size_t i = 0; i < a.length(); ++i) {
            REQUIRE(core::math::isequal(b(i), static_cast<double>(i+1)));
            REQUIRE(core::math::isequal(c(i), static_cast<double>(i+1)));
            REQUIRE(core::math::isequal(d(i), static_cast<double>(i+1)));
        }
    }

    {   /* test unary scalar operators */
        core::math::vec3<double> a(1,2,3);
        a += 2;     /* a=(3,4,5) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<double>(i+1+2)));
        }

        a -= 1;     /* a=(2,3,4) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<double>(i+1+1)));
        }

        a *= 6;     /* a=(12,18,24) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<double>((i+1+1)*6)));
        }

        a /= 3;     /* a=(4,6,8) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<double>((i+1+1)*2)));
        }
    }

    {   /* test unary vector operators */
        core::math::vec3<double> one(1);
        core::math::vec3<double> two = one + one;                      /* + */
        core::math::vec3<double> three = two + two - one;              /* +,- */
        core::math::vec3<double> six = ((three + one) * three) / two;  /* +,*,/ */

        core::math::vec3<double> a(1,2,3);
        a += two;     /* a=(3,4,5) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<double>(i+1+2)));
        }

        a -= one;     /* a=(2,3,4) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<double>(i+1+1)));
        }

        a *= six;     /* a=(12,18,24) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<double>((i+1+1)*6)));
        }

        a /= three;     /* a=(4,6,8) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<double>((i+1+1)*2)));
        }
    }

    {   /* test increment operators */
        core::math::vec3<double> one(1);
        core::math::vec3<double> two(one);
        two++;
        two--;
        ++two;
        ++two;
        core::math::vec3<double> three = two--;
        core::math::vec3<double> six = ((three + one) * three) / two;  /* +,*,/ */

        core::math::vec3<double> a(1,2,3);
        a += two;     /* a=(3,4,5) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<double>(i+1+2)));
        }

        a -= one;     /* a=(2,3,4) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<double>(i+1+1)));
        }

        a *= six;     /* a=(12,18,24) */
        for (size_t i = 0; i < a.length(); i++) {
            REQUIRE(core::math::isequal(a(i), static_cast<double>((i+1+1)*6)));
        }

        a /= three;     /* a=(4,6,8) */
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

#endif /* TEST_CORE_GEOMETRY_VEC3_H_ */