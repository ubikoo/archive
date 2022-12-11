/*
 * test-mat4.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_CORE_GEOMETRY_MAT4_H_
#define TEST_CORE_GEOMETRY_MAT4_H_

/** ---------------------------------------------------------------------------
 * test_mat4<Type>
 * @brief mat4<Type> test client.
 */
template<typename Type>
void test_mat4 (void)
{
    {   /* test default constructor */
        core::math::mat4<Type> a;
        core::math::mat4<Type> b(a);
        core::math::mat4<Type> c;
        core::math::mat4<Type> d = c = b;
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
                REQUIRE(b(i, j) == a(i, j));
                REQUIRE(c(i, j) == a(i, j));
                REQUIRE(d(i, j) == a(i, j));
            }
        }
    }

    {   /* test element constructor */
        core::math::mat4<Type> a(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
        core::math::mat4<Type> b(a);
        core::math::mat4<Type> c;
        core::math::mat4<Type> d = c = b;
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
                REQUIRE(b(i, j) == static_cast<Type>(i*a.dim() + j));
                REQUIRE(c(i, j) == static_cast<Type>(i*a.dim() + j));
                REQUIRE(d(i, j) == static_cast<Type>(i*a.dim() + j));
            }
        }
    }

    {   /* test data constructor */
        Type data[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
        core::math::mat4<Type> a(&data[0]);
        core::math::mat4<Type> b(a);
        core::math::mat4<Type> c;
        core::math::mat4<Type> d = c = b;
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
                REQUIRE(b(i, j) == static_cast<Type>(i*a.dim() + j));
                REQUIRE(c(i, j) == static_cast<Type>(i*a.dim() + j));
                REQUIRE(d(i, j) == static_cast<Type>(i*a.dim() + j));
            }
        }
    }

    {   /* test unary scalar operators */
        Type one = static_cast<Type>(1);
        Type two = static_cast<Type>(2);
        Type three = static_cast<Type>(3);
        Type six = static_cast<Type>(6);

        core::math::mat4<Type> a(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
        a += two;       /* a=(2,3,4,5,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(a(i, j) == static_cast<Type>(i*a.dim() + j + 2));
            }
        }

        a -= one;       /* a=(1,2,3,4,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(a(i, j) == static_cast<Type>(i*a.dim() + j + 1));
            }
        }

        a *= six;       /* a=(6,12,18,24,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(a(i, j) == static_cast<Type>((i*a.dim() + j + 1)*6));
            }
        }

        a /= three;     /* a=(2,4,6,8,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(a(i, j) == static_cast<Type>((i*a.dim() + j + 1)*2));
            }
        }
    }

    {   /* test unary mattor operators */
        core::math::mat4<Type> one(1);
        core::math::mat4<Type> two = one + one;                      /* + */
        core::math::mat4<Type> three = two + two - one;              /* +,- */
        core::math::mat4<Type> six = ((three + one) * three) / two;  /* +,*,/ */

        core::math::mat4<Type> a(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
        a += two;       /* a=(2,3,4,5,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(a(i, j) == static_cast<Type>(i*a.dim() + j + 2));
            }
        }

        a -= one;       /* a=(1,2,3,4,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(a(i, j) == static_cast<Type>(i*a.dim() + j + 1));
            }
        }

        a *= six;       /* a=(6,12,18,24,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(a(i, j) == static_cast<Type>((i*a.dim() + j + 1)*6));
            }
        }

        a /= three;     /* a=(2,4,6,8,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(a(i, j) == static_cast<Type>((i*a.dim() + j + 1)*2));
            }
        }
    }

    {   /* test increment operators */
        core::math::mat4<Type> one(1);
        core::math::mat4<Type> two(one);
        two++;
        two--;
        ++two;
        ++two;
        core::math::mat4<Type> three = two--;
        core::math::mat4<Type> six = ((three + one) * three) / two;  /* +,*,/ */

        core::math::mat4<Type> a(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
        a += two;       /* a=(2,3,4,5,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(a(i, j) == static_cast<Type>(i*a.dim() + j + 2));
            }
        }

        a -= one;       /* a=(1,2,3,4,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(a(i, j) == static_cast<Type>(i*a.dim() + j + 1));
            }
        }

        a *= six;       /* a=(6,12,18,24,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(a(i, j) == static_cast<Type>((i*a.dim() + j + 1)*6));
            }
        }

        a /= three;     /* a=(2,4,6,8,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(a(i, j) == static_cast<Type>((i*a.dim() + j + 1)*2));
            }
        }
    }
}


/** ---------------------------------------------------------------------------
 * test_mat4<float>
 * @brief mat4<float> test client.
 */
template<>
void test_mat4<float> (void)
{
    {   /* test default constructor */
        core::math::mat4<float> a;
        core::math::mat4<float> b(a);
        core::math::mat4<float> c;
        core::math::mat4<float> d = c = b;
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
                REQUIRE(core::math::isequal(b(i, j), a(i, j)));
                REQUIRE(core::math::isequal(c(i, j), a(i, j)));
                REQUIRE(core::math::isequal(d(i, j), a(i, j)));
            }
        }
    }

    {   /* test element constructor */
        core::math::mat4<float> a(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
        core::math::mat4<float> b(a);
        core::math::mat4<float> c;
        core::math::mat4<float> d = c = b;
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
                REQUIRE(core::math::isequal(
                    b(i, j), static_cast<float>(i*a.dim() + j)));
                REQUIRE(core::math::isequal(
                    c(i, j), static_cast<float>(i*a.dim() + j)));
                REQUIRE(core::math::isequal(
                    d(i, j), static_cast<float>(i*a.dim() + j)));
            }
        }
    }

    {   /* test data constructor */
        float data[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
        core::math::mat4<float> a(&data[0]);
        core::math::mat4<float> b(a);
        core::math::mat4<float> c;
        core::math::mat4<float> d = c = b;
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
                REQUIRE(core::math::isequal(
                    b(i, j), static_cast<float>(i*a.dim() + j)));
                REQUIRE(core::math::isequal(
                    c(i, j), static_cast<float>(i*a.dim() + j)));
                REQUIRE(core::math::isequal(
                    d(i, j), static_cast<float>(i*a.dim() + j)));
            }
        }
    }

    {   /* test unary scalar operators */
        float one = static_cast<float>(1);
        float two = static_cast<float>(2);
        float three = static_cast<float>(3);
        float six = static_cast<float>(6);

        core::math::mat4<float> a(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
        a += two;       /* a=(2,3,4,5,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(core::math::isequal(
                   a(i, j), static_cast<float>(i*a.dim() + j + 2)));
            }
        }

        a -= one;       /* a=(1,2,3,4,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(core::math::isequal(
                   a(i, j), static_cast<float>(i*a.dim() + j + 1)));
            }
        }

        a *= six;       /* a=(6,12,18,24,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(core::math::isequal(
                   a(i, j), static_cast<float>((i*a.dim() + j + 1)*6)));
            }
        }

        a /= three;     /* a=(2,4,6,8,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(core::math::isequal(
                   a(i, j), static_cast<float>((i*a.dim() + j + 1)*2)));
            }
        }
    }

    {   /* test unary mattor operators */
        core::math::mat4<float> one(1);
        core::math::mat4<float> two = one + one;                      /* + */
        core::math::mat4<float> three = two + two - one;              /* +,- */
        core::math::mat4<float> six = ((three + one) * three) / two;  /* +,*,/ */

        core::math::mat4<float> a(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
        a += two;       /* a=(2,3,4,5,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(core::math::isequal(
                   a(i, j), static_cast<float>(i*a.dim() + j + 2)));
            }
        }

        a -= one;       /* a=(1,2,3,4,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(core::math::isequal(
                   a(i, j), static_cast<float>(i*a.dim() + j + 1)));
            }
        }

        a *= six;       /* a=(6,12,18,24,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(core::math::isequal(
                   a(i, j), static_cast<float>((i*a.dim() + j + 1)*6)));
            }
        }

        a /= three;     /* a=(2,4,6,8,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(core::math::isequal(
                   a(i, j), static_cast<float>((i*a.dim() + j + 1)*2)));
            }
        }
    }

    {   /* test increment operators */
        core::math::mat4<float> one(1);
        core::math::mat4<float> two(one);
        two++;
        two--;
        ++two;
        ++two;
        core::math::mat4<float> three = two--;
        core::math::mat4<float> six = ((three + one) * three) / two;  /* +,*,/ */

        core::math::mat4<float> a(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
        a += two;       /* a=(2,3,4,5,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(core::math::isequal(
                   a(i, j), static_cast<float>(i*a.dim() + j + 2)));
            }
        }

        a -= one;       /* a=(1,2,3,4,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(core::math::isequal(
                   a(i, j), static_cast<float>(i*a.dim() + j + 1)));
            }
        }

        a *= six;       /* a=(6,12,18,24,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(core::math::isequal(
                   a(i, j), static_cast<float>((i*a.dim() + j + 1)*6)));
            }
        }

        a /= three;     /* a=(2,4,6,8,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(core::math::isequal(
                   a(i, j), static_cast<float>((i*a.dim() + j + 1)*2)));
            }
        }
    }
}


/** ---------------------------------------------------------------------------
 * test_mat4<double>
 * @brief mat4<double> test client.
 */
template<>
void test_mat4<double> (void)
{
    {   /* test default constructor */
        core::math::mat4<double> a;
        core::math::mat4<double> b(a);
        core::math::mat4<double> c;
        core::math::mat4<double> d = c = b;
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
                REQUIRE(core::math::isequal(b(i, j), a(i, j)));
                REQUIRE(core::math::isequal(c(i, j), a(i, j)));
                REQUIRE(core::math::isequal(d(i, j), a(i, j)));
            }
        }
    }

    {   /* test element constructor */
        core::math::mat4<double> a(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
        core::math::mat4<double> b(a);
        core::math::mat4<double> c;
        core::math::mat4<double> d = c = b;
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
                REQUIRE(core::math::isequal(
                    b(i, j), static_cast<double>(i*a.dim() + j)));
                REQUIRE(core::math::isequal(
                    c(i, j), static_cast<double>(i*a.dim() + j)));
                REQUIRE(core::math::isequal(
                    d(i, j), static_cast<double>(i*a.dim() + j)));
            }
        }
    }

    {   /* test data constructor */
        double data[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
        core::math::mat4<double> a(&data[0]);
        core::math::mat4<double> b(a);
        core::math::mat4<double> c;
        core::math::mat4<double> d = c = b;
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
                REQUIRE(core::math::isequal(
                    b(i, j), static_cast<double>(i*a.dim() + j)));
                REQUIRE(core::math::isequal(
                    c(i, j), static_cast<double>(i*a.dim() + j)));
                REQUIRE(core::math::isequal(
                    d(i, j), static_cast<double>(i*a.dim() + j)));
            }
        }
    }

    {   /* test unary scalar operators */
        double one = static_cast<double>(1);
        double two = static_cast<double>(2);
        double three = static_cast<double>(3);
        double six = static_cast<double>(6);

        core::math::mat4<double> a(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
        a += two;       /* a=(2,3,4,5,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(core::math::isequal(
                   a(i, j), static_cast<double>(i*a.dim() + j + 2)));
            }
        }

        a -= one;       /* a=(1,2,3,4,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(core::math::isequal(
                   a(i, j), static_cast<double>(i*a.dim() + j + 1)));
            }
        }

        a *= six;       /* a=(6,12,18,24,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(core::math::isequal(
                   a(i, j), static_cast<double>((i*a.dim() + j + 1)*6)));
            }
        }

        a /= three;     /* a=(2,4,6,8,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(core::math::isequal(
                   a(i, j), static_cast<double>((i*a.dim() + j + 1)*2)));
            }
        }
    }

    {   /* test unary mattor operators */
        core::math::mat4<double> one(1);
        core::math::mat4<double> two = one + one;                      /* + */
        core::math::mat4<double> three = two + two - one;              /* +,- */
        core::math::mat4<double> six = ((three + one) * three) / two;  /* +,*,/ */

        core::math::mat4<double> a(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
        a += two;       /* a=(2,3,4,5,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(core::math::isequal(
                   a(i, j), static_cast<double>(i*a.dim() + j + 2)));
            }
        }

        a -= one;       /* a=(1,2,3,4,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(core::math::isequal(
                   a(i, j), static_cast<double>(i*a.dim() + j + 1)));
            }
        }

        a *= six;       /* a=(6,12,18,24,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(core::math::isequal(
                   a(i, j), static_cast<double>((i*a.dim() + j + 1)*6)));
            }
        }

        a /= three;     /* a=(2,4,6,8,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(core::math::isequal(
                   a(i, j), static_cast<double>((i*a.dim() + j + 1)*2)));
            }
        }
    }

    {   /* test increment operators */
        core::math::mat4<double> one(1);
        core::math::mat4<double> two(one);
        two++;
        two--;
        ++two;
        ++two;
        core::math::mat4<double> three = two--;
        core::math::mat4<double> six = ((three + one) * three) / two;  /* +,*,/ */

        core::math::mat4<double> a(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
        a += two;       /* a=(2,3,4,5,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(core::math::isequal(
                   a(i, j), static_cast<double>(i*a.dim() + j + 2)));
            }
        }

        a -= one;       /* a=(1,2,3,4,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(core::math::isequal(
                   a(i, j), static_cast<double>(i*a.dim() + j + 1)));
            }
        }

        a *= six;       /* a=(6,12,18,24,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(core::math::isequal(
                   a(i, j), static_cast<double>((i*a.dim() + j + 1)*6)));
            }
        }

        a /= three;     /* a=(2,4,6,8,...) */
        for (size_t i = 0; i < a.dim(); ++i) {
            for (size_t j = 0; j < a.dim(); ++j) {
               REQUIRE(core::math::isequal(
                   a(i, j), static_cast<double>((i*a.dim() + j + 1)*2)));
            }
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

#endif /* TEST_CORE_GEOMETRY_VECMAT_ */