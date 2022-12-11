/*
 * random.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef CORE_RANDOM_H_
#define CORE_RANDOM_H_

#include "core/core.hpp"

namespace core {
namespace math {

/** ---- Base random number generator engine ----------------------------------
 * struct RngEngine
 *
 * @brief Base random number generator engine. It implements a device
 * random number generator and a random prime number generator
 *
 * RngEngine is the base class of a curiously recursive template pattern.
 * The derived children are random number generator engines used as the
 * template typename parameter T of the RngEngine base class.
 *
 * Each derived random number generator engine implements the following:
 *
 * init     initialise the random number generator engine state.
 * rand32   sample a 32-bit random number.
 * rand64   sample a 64-bit random number.
 */
template<typename T>
struct RngEngine {
    /* Constructor/destructor */
    RngEngine() = default;
    ~RngEngine() = default;

    /* Copy constructor/assignment */
    RngEngine(const RngEngine &other) = default;
    RngEngine &operator=(const RngEngine &other) = default;

    /* Move constructor/assignment */
    RngEngine(RngEngine &&other) = default;
    RngEngine &operator=(RngEngine &&other) = default;

    /*
     * Random number generator engine functions.
     */
    uint32_t randev(void);
    uint32_t randprime(void);
    bool isprime(const uint32_t n);
    uint32_t nearbyprime(const uint32_t num);

    /*
     * Random number generator engine derived functions.
     */
    T *engine(void) { return static_cast<T *>(this); }
    void init(void) { engine()->init(); }
    uint32_t rand32 (void) { return engine()->rand32(); }
    uint64_t rand64 (void) { return engine()->rand64(); }
};

/**
 * RngEngine<T>::randev
 * @brief Read a 32-bit random number from the random device.
 */
template<typename T>
uint32_t RngEngine<T>::randev(void) {
    uint32_t number;

    core::FileIn fp;
    try {
        fp.open("/dev/urandom", core::FileIn::Binary);
        core_assert_(fp.read(&number, sizeof(uint32_t)), "I/O error");
        fp.close();
    } catch (std::exception& e) {
        fp.close();
        core_throw_(e.what());
    }

    return number;
}

/**
 * RngEngine<T>::randprime
 * @brief Compute the nearest prime smaller than a 32-bit random number.
 */
template<typename T>
uint32_t RngEngine<T>::randprime(void) {
    return nearbyprime(randev());
}

/**
 * RngEngine<T>::isprime
 *
 * @brief Is the specified number n a prime number?
 * A prime number is a natural number greater than 1 that cannot
 * be formed by multiplying two smaller natural numbers
 *
 * Search for any divisor p < n. If none found then n is a prime.
 * We only need to test i < sqrt(n) (+1 to ensure correct counting).
 *
 * If n = p*q, then p ≤ sqrt(n) or q ≤ sqrt(n), otherwise p*q > n.
 * In the case where p = q, then
 *
 * n = p*p and p = sqrt(n).
 *
 * In the case where p != q, then either
 *
 * p > sqrt(n) and q < sqrt(n)
 * or conversely
 * p < sqrt(n) and q > sqrt(n)
 *
 * If there is no divisor p < sqrt(n) : n%p == 0, then n must be
 * a prime.
 *
 * Every even number larger than 2 is not prime, because 2 divides
 * all even numbers. If 2 does not divide n, then every other even
 * number smaller than n will also not.
 */
template<typename T>
bool RngEngine<T>::isprime(const uint32_t num) {
    /* If num is even, then it's not a prime */
    if (num%2 == 0) {
        return false;
    }

    uint32_t sqn = (uint32_t) std::sqrt((double) num) + 1;
    for (uint32_t p = 3; p < sqn; p += 2) {
        /* If p is a divisor, num is not prime */
        if (num%p == 0) {
            return false;
        }
    }

    /* Otherwise, num is necessarily prime */
    return true;
}

/**
 * RngEngine<T>::nearbyprime
 * @brief Find the nearest prime smaller or equal than n.
 */
template<typename T>
uint32_t RngEngine<T>::nearbyprime(const uint32_t num) {
    /*
     * Go over all the numbers smaller than n and test their primality
     */
    for (uint32_t n = num; n >= 3; --n) {
        if (isprime(n))
            return n;
    }

    /*
     * If num = 1 or num = 2 or num = 3, then num is a prime
     */
    return num;
}

/** ---- Kiss random number generator engine ----------------------------------
 * struct RngKiss
 * @brief Random number generator engine based on the 64-bit variant of
 * George Marsaglia's KISS generator, and designed by David Jones.
 *
 * @see http://www.cs.ucl.ac.uk/staff/d.jones/GoodPracticeRNG.pdf
 */
struct RngKiss : RngEngine<RngKiss> {
    /*
     * Random number generator engine constants.
     *
     * Linear congruential generator is defined by
     *
     * Xn+1 = (m1*Xn + m2) mod m, where m = 2^64-1.
     *
     * Multiply-with-carry generator 1
     *
     * Xn+1 = (m3*Xn + c) mod m, and c = Xn+1 >> 32
     *
     * Multiply-with-carry generator 2
     *
     * Xn+1 = (m4*Xn + c) mod m, and c = Xn+1 >> 32
     */
    const uint64_t m1_ = 1490024343005336237ULL;
    const uint64_t m2_ = 123456789ULL;
    const uint64_t m3_ = 4294584393ULL;
    const uint64_t m4_ = 4246477509ULL;

    const uint64_t seed_x_  = 123456789123ULL;
    const uint64_t seed_y_  = 987654321987ULL;
    const uint32_t seed_z1_ = 43219876U;
    const uint32_t seed_c1_ = 6543217U;
    const uint32_t seed_z2_ = 21987643U;
    const uint32_t seed_c2_ = 1732654U;
    const uint32_t seed_cmax_ = 698769069U;

    const uint32_t warmup_steps_ = 1024;

    /*
     * Random number generator engine state
     */
    uint64_t x_;        /* linear congruential generator */
    uint64_t y_;        /* xor-shift generator */
    uint32_t z1_;       /* multiply-with-carry generator */
    uint32_t c1_;
    uint32_t z2_;       /* multiply-with-carry generator */
    uint32_t c2_;

    /*
     * Constructor/destructor
     */
    RngKiss()
        : x_(seed_x_)
        , y_(seed_y_)
        , z1_(seed_z1_)
        , c1_(seed_c1_)
        , z2_(seed_z2_)
        , c2_(seed_c2_) {
        /* Warmup the random number generator engine */
        for (uint32_t i = 0; i < warmup_steps_; ++i) {
             uint64_t core_unused_(n) = rand64();   /* or(void) rand64() */
        }
    }
    ~RngKiss() = default;

    /*
     * Copy constructor/assignment
     */
    RngKiss(const RngKiss &other)
        : x_(other.x_)
        , y_(other.y_)
        , z1_(other.z1_)
        , c1_(other.c1_)
        , z2_(other.z2_)
        , c2_(other.c2_) {}

    RngKiss &operator=(const RngKiss &other) {
        if (this == &other) {
            return *this;
        }

        x_  = other.x_;
        y_  = other.y_;
        z1_ = other.z1_;
        c1_ = other.c1_;
        z2_ = other.z2_;
        c2_ = other.c2_;

        return *this;
    }

    /*
     * Move constructor/assignment
     */
    RngKiss(RngKiss &&other)
        : x_(std::move(other.x_))
        , y_(std::move(other.y_))
        , z1_(std::move(other.z1_))
        , c1_(std::move(other.c1_))
        , z2_(std::move(other.z2_))
        , c2_(std::move(other.c2_)) {

        other.x_  = seed_x_;
        other.y_  = seed_y_;
        other.z1_ = seed_z1_;
        other.c1_ = seed_c1_;
        other.z2_ = seed_z2_;
        other.c2_ = seed_c2_;
    }

    RngKiss &operator=(RngKiss &&other) {
        if (this == &other) {
            return *this;
        }

        x_  = std::move(other.x_);
        y_  = std::move(other.y_);
        z1_ = std::move(other.z1_);
        c1_ = std::move(other.c1_);
        z2_ = std::move(other.z2_);
        c2_ = std::move(other.c2_);

        other.x_  = seed_x_;
        other.y_  = seed_y_;
        other.z1_ = seed_z1_;
        other.c1_ = seed_c1_;
        other.z2_ = seed_z2_;
        other.c2_ = seed_c2_;

        return *this;
    }

    /*
     * Random number generator engine initialisation and sampling functions.
     */
    void init(void);
    uint32_t rand32 (void);
    uint64_t rand64 (void);
};

/**
 * RngKiss::init
 * @brief Initialize the KISS random number generator engine.
 *
 * Four random seeds(x, y, z1, c2), are necessary for the 32-bit
 * generator and six(x, y, z1, c1, z2, c2) for the 64-bit generator.
 *
 * 0 < x  < 2^32,       linear congruential generator
 * 0 < y  < 2^32,       xor-shift register generator
 * 0 < z1 < 2^32,       multiply-with-carry generator 1
 * 0 < c1 < 698769069
 * 0 < z2 < 2^32,       multiply-with-carry generator 2
 * 0 < c2 < 698769069
 *
 * @note  Do not set y = 0 and avoid x = z1 = c1 = z2 = c2 = 0.
 */
core_inline_
void RngKiss::init(void)
{
    /*
     * Seed the linear congruential generator state
     */
    do {
        x_ = ((uint64_t) randprime() << 0  & 0x00000000FFFFFFFFULL) |
             ((uint64_t) randprime() << 32 & 0xFFFFFFFF00000000ULL);
    } while (x_ < 3);

    /*
     * Seed the xor-shift register generator state
     */
    do {
        y_ = ((uint64_t) randprime() << 0  & 0x00000000FFFFFFFFULL) |
             ((uint64_t) randprime() << 32 & 0xFFFFFFFF00000000ULL);
    } while (y_ < 3);

    /*
     * Seed the first multiply-with-carry generator state
     */
    do {
        z1_ = randprime();
    } while (z1_ < 3);


    do {
        c1_ = randprime();
    } while (c1_ < 3 || c1_ >= seed_cmax_);

    /*
     * Seed the second multiply-with-carry generator state
     */
    do {
        z2_ = randprime();
    } while (z2_ < 3);


    do {
        c2_ = randprime();
    } while (c2_ < 3 || c2_ >= seed_cmax_);

    /*
     * Warmup the random number generator engine
     */
    for (uint32_t i = 0; i < warmup_steps_; ++i) {
       (void) rand64();    /* ignore output by casting it to(void) */
    }
}

/**
 * RngKiss::rand32
 * @brief Long period 32-bit KISS random number generator engine.
 */
core_inline_
uint32_t RngKiss::rand32 (void)
{
    /* Linear congruential generator */
    x_ =  m1_ * x_ + m2_;

    /* Xorshift register generator - do not set y = 0*/
    y_ ^= y_ << 21;
    y_ ^= y_ >> 17;
    y_ ^= y_ << 30;

    /* First multiply-with-carry generator - avoid z1 = c1 = 0 */
    uint64_t t1 = m3_ * z1_ + c1_;
    c1_ = (uint32_t) (t1 >> 8) + (uint32_t) (t1 >> 24); /* or(t1 >> 32) */
    z1_ = (uint32_t) (t1 >> 8);

    /*
     * Mix the multiply-with-carry values in a
     * 32-bit word and return the 32-bit result.
     */
    uint32_t z_ = (uint32_t) (z1_ >> 8) + (uint32_t) (z1_ >> 24);

    return ((uint32_t)(x_ >> 8) + (uint32_t)(x_ >> 24) + (uint32_t)(y_) + z_);
}

/**
 * RngKiss::rand64
 * @brief Long period 64-bit KISS random number generator engine.
 */
core_inline_
uint64_t RngKiss::rand64 (void)
{
    /* Linear congruential generator */
    x_ =  m1_ * x_ + m2_;

    /* Xorshift register generator - do not set y = 0*/
    y_ ^= y_ << 21;
    y_ ^= y_ >> 17;
    y_ ^= y_ << 30;

    /* First multiply-with-carry generator - avoid z1 = c1 = 0 */
    uint64_t t1 = m3_ * z1_ + c1_;
    c1_ = (uint32_t) (t1 >> 8) + (uint32_t) (t1 >> 24); /* or(t1 >> 32) */
    z1_ = (uint32_t) (t1 >> 8);

    /* Second multiply-with-carry generator - avoid z2 = c2 = 0 */
    uint64_t t2 = m4_ * z2_ + c2_;
    c2_ = (uint32_t) (t2 >> 8) + (uint32_t) (t2 >> 24); /* or(t2 >> 32) */
    z2_ = (uint32_t) (t2 >> 8);

    /*
     * Mix the multiply-with-carry values in a
     * 64-bit word and return the 64-bit result.
     */
    uint32_t r1 = (uint32_t) (z1_ >> 8) + (uint32_t) (z1_ >> 24);
    uint32_t r2 = (uint32_t) (z2_ >> 8) + (uint32_t) (z2_ >> 24);

    uint64_t z_ = (((uint64_t) (r1) << 0  & 0x00000000FFFFFFFFULL) |
                   ((uint64_t) (r2) << 32 & 0xFFFFFFFF00000000ULL));

    return (x_ + y_ + z_);
}

/** ---- Multiply-with-Carry random number engine -----------------------------
 *
 * struct RngCmwc
 *
 * @brief Random number generator engine based on the 32-bit variant of
 * George Marsaglia's complementary multiply-with-carry CMWC256
 * generator - Period 2^8222.
 *
 * @see
 * https://groups.google.com/d/msg/sci.math/k3kVM8KwR-s/jxPdZl8XWZkJ
 * https://groups.google.com/d/msg/comp.lang.c/qZFQgKRCQGg/rmPkaRHqxOMJ
 * http://groups.google.co.uk/group/sci.math.num-analysis/msg/eb4ddde782b17051
 * http://groups.google.co.uk/group/sci.math/msg/9959175f66dd138f
 * http://groups.google.co.uk/group/sci.math/msg/5d891ca5727b97d2
 */
struct RngCmwc : RngEngine<RngCmwc> {
    /*
     * Random number generator engine constants
     */
    const uint32_t seed_c_ = 362436U;
    const uint32_t seed_cmax_ = 809430660U;
    const uint32_t pool_size_ = 256;
    const uint32_t warmup_steps_ = 1024;

    /*
     * Random number generator engine state
     */
    uint32_t Q_[256];               /* pool state with a 256 cycle */
    uint32_t c_ = seed_c_;          /* carry state */
    uint8_t ix_ = pool_size_ - 1;   /* pool index */

    /*
     * Constructor/destructor
     */
    RngCmwc() {
        /* Seed the pool state */
        for (uint32_t i = 0; i < pool_size_; ++i) {
            do {
                Q_[i] = randprime();
            } while (Q_[i] < 3);
        }

        /* Warmup the random number generator engine */
        for (uint32_t i = 0; i < warmup_steps_; ++i) {
             uint64_t core_unused_(n) = rand64();   /* or(void) rand64() */
        }
    }
    ~RngCmwc() = default;

    /*
     * Copy constructor/assignment
     */
    RngCmwc(const RngCmwc &other)
        : c_(other.c_)
        , ix_(other.ix_) {
        std::memcpy(
            (void *) Q_,
            (const void *) other.Q_,
            pool_size_ * sizeof(uint32_t));
    }

    RngCmwc &operator=(const RngCmwc &other) {
        if (this == &other) {
            return *this;
        }

        c_  = other.c_;
        ix_ = other.ix_;
        std::memcpy(
            (void *) Q_,
            (const void *) other.Q_,
            pool_size_ * sizeof(uint32_t));

        return *this;
    }

    /*
     * Move constructor/assignment
     */
    RngCmwc(RngCmwc &&other)
        : c_(std::move(other.c_))
        , ix_(std::move(other.ix_)) {
        std::memcpy(
            (void *) Q_,
            (const void *) other.Q_,
            pool_size_ * sizeof(uint32_t));
    }

    RngCmwc &operator=(RngCmwc &&other) {
        if (this == &other) {
            return *this;
        }

        c_  = std::move(other.c_);
        ix_ = std::move(other.ix_);
        std::memcpy(
            (void *) Q_,
            (const void *) other.Q_,
            pool_size_ * sizeof(uint32_t));

        return *this;
    }

    /*
     * Random number generator engine initialisation and sampling functions.
     */
    void init(void);
    uint32_t rand32 (void);
    uint64_t rand64 (void);
};

/**
 * RngCmwc::init
 * @brief Initialize the MWC256 random number generator engine.
 * Choose a random initial carry value c < 809430660 and 256 random
 * 32-bit prime integers for the state Q[].
 */
core_inline_
void RngCmwc::init(void)
{
    /*
     * Seed the carry state.
     */
    do {
        c_ = randprime();
    } while (c_ < 3 || c_ >= seed_cmax_);

    /*
     * Seed the pool state
     */
    for (uint32_t i = 0; i < pool_size_; ++i) {
        do {
            Q_[i] = randprime();
        } while (Q_[i] < 3);
    }

    /*
     * Warmup the random number generator engine
     */
    for (uint32_t i = 0; i < warmup_steps_; ++i) {
       (void) rand64();    /* ignore output by casting it to(void) */
    }
}

/**
 * RngCmwc::rand32
 * @brief Long period 32-bit MWC256 random number generator engine
 * based on usenet posting by G. Marsaglia.
 */
core_inline_
uint32_t RngCmwc::rand32 (void)
{
    const uint64_t m = 1540315826ULL;       /* or 809430660ULL */

    uint64_t t = m * Q_[++ix_] + c_;
    c_ = (uint32_t) (t >> 8) + (uint32_t) (t >> 24);  /* or c = (t >> 32) */

    uint32_t x = t + c_;
    if (x < c_) {
        x++;
        c_++;
    }

    return (Q_[ix_] = 0xFFFFFFFEU - x);     /* Q[ix] = (2^32 - 1) - x */
}

/**
 * RngCmwc::rand64
 * @brief Long period 64-bit MWC256 random number generator engine
 * represented as bitwise pair of two 32-bit random samples.
 */
core_inline_
uint64_t RngCmwc::rand64 (void)
{
    const uint64_t m1 = 4294584393ULL;    /* 2^32 */
    const uint64_t m2 = 4246477509ULL;

    uint64_t z1 = m1 * rand32() + c_;
    uint32_t carry = (uint32_t) (z1 >> 8) + (uint32_t) (z1 >> 24);
    uint64_t z2 = m2 * rand32() + carry;

    return (((uint64_t) (z1) << 0  & 0x00000000FFFFFFFFULL) |
            ((uint64_t) (z2) << 32 & 0xFFFFFFFF00000000ULL));
}

/** ---- Random number generator ----------------------------------------------
 * struct RngDist
 * @brief Random number distribution. Sample random variates from a
 * given distribution using a random number generator engine.
 */
struct RngDist {
    /* Member variables */
    bool hascache_ = false;   /* normal random deviate cache state */
    double cache_ = 0.0;    /* stored normal random deviate */

    /* Constructor/destructor */
    RngDist() = default;
    ~RngDist() = default;

    /* Copy constructor/assignment */
    RngDist(const RngDist &other) = default;
    RngDist &operator=(const RngDist &other) = default;

    /* Move constructor/assignment */
    RngDist(RngDist &&other) = default;
    RngDist &operator=(RngDist &&other) = default;

    /*
     * Generate 64-bit floating point random number from
     * a uniform distribution in the interval [a,b].
     */
    template<typename T>
    double urand(RngEngine<T> &engine, double a = 0.0, double b = 1.0) {
        const uint64_t max_uint64 = (uint64_t) (-1);
        double r = (double) engine.rand64() / (double) max_uint64;
        return (a + (b-a)*r);
    }

    /*
     * Generate 64-bit floating point random number from a Gaussian
     * normal distribution with average mu and standard deviation sdev.
     */
    template<typename T>
    double nrand(RngEngine<T> &engine, double mu = 0.0, double sdev = 1.0);

    /*
     * Generate 32-bit integer random number from a
     * uniform distribution in the interval [a,b].
     */
    template<typename T>
    uint32_t irand32 (RngEngine<T> &engine,
                      uint32_t a = 0,
                      uint32_t b = (uint32_t) (-1)) {
        double delta = (double) (b - a + 1);
        double n = (double) a + std::floor(urand(engine)*delta);
        return ((uint32_t) n);
    }

    /*
     * Generate 64-bit integer random number from a
     * uniform distribution in the interval [a,b].
     */
    template<typename T>
    uint64_t irand64 (RngEngine<T> &engine,
                      uint64_t a = 0,
                      uint64_t b = (uint64_t) (-1)) {
        double delta = (double) (b - a + 1);
        double n = (double) a + std::floor(urand(engine)*delta);
        return ((uint64_t) n);
    }
};

/**
 * RngDist::nrand
 * @brief Generate 64-bit floating point random number from a Gaussian
 * normal distribution with average mu and standard deviation sdev.
 *
 * If Z is a standard normal deviate with zero mean and unit variance,
 * then X = sig*Z + mu will be a normal deviate with mean mu and
 * variance sig^2.
 *
 * Sample a random deviate Z from a standard normal distribution, i.e.,
 * a Gaussian distribution with zero mean and unit variance using the
 * Box-Muller algorithm.
 *
 * @see http://en.literateprograms.org/Box-Muller_transform_(C)
 * Compute the normal deviate X using the corresponding linear relation.
 */
template<typename T>
double RngDist::nrand(RngEngine<T> &engine, double mu, double sdev)
{
    double value = 0.0;
    if (hascache_) {
        /*
         * Get the stored normal random deviate and reset cache state.
         */
        value = cache_;
        hascache_ = false;
    } else {
        /*
         * Sample uniformly a random point in the unit circle by computing
         * two uniform random values in the interval [-1.0, 1.0].
         */
        double x, y, r;
        do {
            x = urand(engine, -1.0, 1.0);
            y = urand(engine, -1.0, 1.0);
            r = x*x + y*y;
        } while (math::isequal(r, 0.0) || !math::isless(r, 1.0));

        double d = std::sqrt(-2.0*std::log(r) / r); /* Box-Muller transform */
        value  = x * d;                 /* Use the first random deviate */
        cache_ = y * d;                 /* Store the second random deviate */
        hascache_ = true;
    }

    return (sdev*value + mu);
}

/** ---- Random number generator engine factory function ----------------------
 * make_array_rng<T>
 * @brief Create an array of count random number generator engines - RngKiss
 * or RngCmwc - and initialize them.
 */
template<typename T = core::math::RngKiss>
core::align_unique_array<T> make_array_rng(size_t count)
{
    core_assert_(count > 0, "invalid array size");

    core::align_unique_array<T> array = core::make_align_unique_array<T>(count);
    for (size_t i = 0; i < count; ++i) {
        (*array)[i].init();
    }
    return array;
}

/**
 * make_vector_rng<T>
 * @brief Create a vector of count random number generator engines - RngKiss
 * or RngCmwc - and initialize them.
 */
template<typename T = core::math::RngKiss>
std::vector<T> make_vector_rng(size_t count)
{
    core_assert_(count > 0, "invalid array size");

    std::vector<T> array(count);
    for (size_t i = 0; i < array.size(); ++i) {
        array[i].init();
    }
    return array;
}

/**
 * make_array_rng_dist
 * @brief Create an array of count RngDist objects and initialize them.
 */
core_inline_
core::align_unique_array<core::math::RngDist> make_array_rng_dist(size_t count)
{
    core_assert_(count > 0, "invalid array size");
    return core::make_align_unique_array<core::math::RngDist>(count);
}

/**
 * make_vector_rng_dist
 * @brief Create an array of count RngDist objects and initialize them.
 */
core_inline_
std::vector<core::math::RngDist> make_vector_rng_dist(size_t count)
{
    core_assert_(count > 0, "invalid array size");
    return std::vector<core::math::RngDist>(count);
}

} /* namespace math */
} /* namespace core */

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

#endif /* CORE_RANDOM_H_ */
