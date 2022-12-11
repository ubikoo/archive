/*
 * sde-ito.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ALGO_NUMERIC_SDE_ITO_H_
#define ALGO_NUMERIC_SDE_ITO_H_

#include "atto/core/core.hpp"
#include "atto/math/math.hpp"
#include "numeric/array/array.hpp"
#include "numeric/linalg/linalg.hpp"
#include "numeric/random.hpp"

namespace algo {

/** ---- Sde Ito stochastic integrator ----------------------------------------
 * struct SdeIto
 * @brief Solve the set of sdes of the type
 *
 * dx(t) = f[x(t)]*dt + g[x(t)]*dw
 *
 * where dt is the time step size, f(x) is the drift term and g(x) is the
 * diffusion term. The term dw is a Wiener process. It is characterized
 * by [w(t), t > 0] and has stationary independent increments dw, where
 * dw is a random Gaussian variable with mean zero and standard deviation
 * equal to sqrt(dt),
 *
 * dw = sqrt(dt) * N(0,1)
 *
 * <[dw]> = 0
 * <[dw]^2> = dt
 *
 * The integration step uses the Ito interpration of
 * stochastic differential equations(Euler-Maruyama Method):
 *
 * x(t+dt) = x(t) + f[x(t)]*dt + g[x(t)]*dw
 *
 * dx = x(n+1) - x(n)
 * dt = t(n+1) - t(n)
 * dw = w(n+1) - w(n) = sqrt(dt)*N(0,1)
 *
 * @see Stochastic algorithms for discontinuous multiplicative white noise,
 * Physical Review E, 81, 032104, 2010.
 */
template<typename T = double,
         typename R = atto::math::rng::Kiss,
         bool IsPar = true>
struct SdeIto {
    static_assert(std::is_floating_point<T>::value, "non floating point type");

    /* SdeIto members */
    SdeWiener<T,R,IsPar> wiener_;   /* Wiener process generator */
    size_t neq_;                    /* number of equations */
    Vector<T> dw_;                  /* Wiener process */

    /* Constructor/destructor */
    SdeIto(size_t neq)
        : wiener_(SdeWiener<T,R,IsPar>())
        , neq_(neq)
        , dw_(Vector<T>(neq)) {}
    ~SdeIto() = default;

    /* Copy constructor/assignment */
    SdeIto(const SdeIto &other) = delete;
    SdeIto &operator=(const SdeIto &other) = delete;

    /* SdeIto functions */
    template<typename Deriv>
    void init(Deriv deriv,
              Vector<T> & core_restrict(x),
              Vector<T> & core_restrict(f),
              Vector<T> & core_restrict(g));
    template<typename Deriv>
    void step(Deriv deriv,
              Vector<T> & core_restrict(x),
              Vector<T> & core_restrict(f),
              Vector<T> & core_restrict(g),
              T dt);
};

/**
 * SdeIto<T,R,IsPar>::init
 * @brief Initialize the Sde solver. The derivative operator deriv is a functor
 * with two functions:
 *  drift(x,f)  compute the drift derivative term in the sde, f[x(t)].
 *  diff(x,g)   compute the diffusion derivative term in the sde, g[x(t)].
 */
template<typename T,
         typename R,
         bool IsPar>
template<typename Deriv>
void SdeIto<T,R,IsPar>::init(
    Deriv deriv,
    Vector<T> & core_restrict(x),
    Vector<T> & core_restrict(f),
    Vector<T> & core_restrict(g))
{
    core_assert(x.n1() == neq_ &&
                f.n1() == neq_ &&
                g.n1() == neq_,
                "invalid dimensions");

    /*
     * Drift and diffusion terms, f[x(t)] and g[x(t)]
     */
    deriv.drift(x, f);
    deriv.diff(x, g);
}

/**
 * SdeIto<T,R,IsPar>::step
 * @brief Sde integration stepper using the Ito interpration of stochastic
 * differential equations (Euler-Maruyama Method).
 */
template<typename T,
         typename R,
         bool IsPar>
template<typename Deriv>
void SdeIto<T,R,IsPar>::step(
    Deriv deriv,
    Vector<T> & core_restrict(x),
    Vector<T> & core_restrict(f),
    Vector<T> & core_restrict(g),
    T dt)
{
    core_assert(x.n1() == neq_ &&
                f.n1() == neq_ &&
                g.n1() == neq_,
                "invalid dimensions");

    /*
     * Wiener stochastic process, dw = sqrt(dt) * N(0,1)
     */
    wiener_(dt, dw_);

    /*
     * Main time step, x(t+dt) = x(t) + (dt * f(t)) + (dw * g(t))
     */
    core_pragma_omp(parallel for if(IsPar) default(none) \
        shared(x, f, g, dt) schedule(static))
    for (size_t i = 0; i < neq_; i++) {
        x(i) += dt * f(i) + dw_(i) * g(i);
    } /* omp parallel */

    /*
     * Compute drift and diffusion terms, f[x(t+dt)] and g[x(t+dt)]
     */
    deriv.drift(x, f);
    deriv.diff(x, g);
}

} /* namespace algo */

#endif /* ALGO_NUMERIC_SDE_ITO_H_ */
