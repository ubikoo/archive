/*
 * ode-gauss.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ALGO_NUMERIC_ODE_GAUSS_H_
#define ALGO_NUMERIC_ODE_GAUSS_H_

#include "atto/core/core.hpp"
#include "atto/math/math.hpp"
#include "numeric/array/array.hpp"
#include "numeric/linalg/linalg.hpp"

namespace algo {

/** ---- OdeGauss integration solver ------------------------------------------
 * struct OdeGauss
 * @brief Solve the set of odes
 *
 * dx
 * -- = f(x(t))
 * dt
 *
 * using the implicit Gauss-Legendre collocation algorithm:
 *
 * x(t+dt) = x(t) + dt*f(t+dt/2, (x(t), x(t+dt)/2)
 *
 * For each time step, the solution is computed using a simple fixed point
 * iteration. The error and the and max number of iteration steps are part
 * of the integrator state.
 *
 * The time derivative of x at time t, f(x(t)), is computed from the derivative
 * operator Deriv. By default Deriv is a noop.
 */
template<typename T = double, bool IsPar = true>
struct OdeGauss {
    static_assert(std::is_floating_point<T>::value, "non floating point type");

    /* Ode member variables. */
    size_t neq_;        /* system dimension */
    double maxerr_;     /* max error value */
    size_t maxiter_;    /* max number of iterations */
    Vector<T> zval_;
    Vector<T> zmid_;
    Vector<T> znew_;
    Vector<T> dzdt_;

    /* Constructor/destructor */
    OdeGauss(size_t neq,
              double maxerr = std::sqrt(std::numeric_limits<double>::epsilon()),
              size_t maxiter = 16)
        : neq_(neq)
        , maxerr_(maxerr)
        , maxiter_(maxiter)
        , zval_(Vector<T>(neq))
        , zmid_(Vector<T>(neq))
        , znew_(Vector<T>(neq))
        , dzdt_(Vector<T>(neq)) {}
    ~OdeGauss() = default;

    /* Copy constructor/assignment */
    OdeGauss(const OdeGauss &other)
        : neq_(neq_)
        , maxerr_(other.maxerr_)
        , maxiter_(other.maxiter_)
        , zval_(other.zval_)
        , zmid_(other.zmid_)
        , znew_(other.znew_)
        , dzdt_(other.dzdt_) {}

    OdeGauss &operator=(const OdeGauss &other) {
        if (this == &other) {
            return *this;
        }

        neq_ = neq_;
        maxerr_ = other.maxerr_;
        maxiter_ = other.maxiter_;
        zval_ = other.zval_;
        zmid_ = other.zmid_;
        znew_ = other.znew_;
        dzdt_ = other.dzdt_;

        return *this;
    }

    /* Move constructor/assignment */
    OdeGauss(OdeGauss &&other)
        : neq_(std::move(neq_))
        , maxerr_(std::move(other.maxerr_))
        , maxiter_(std::move(other.maxiter_))
        , zval_(std::move(other.zval_))
        , zmid_(std::move(other.zmid_))
        , znew_(std::move(other.znew_))
        , dzdt_(std::move(other.dzdt_)) {}

    OdeGauss &operator=(OdeGauss &&other) {
        if (this == &other) {
            return *this;
        }

        neq_ = std::move(neq_);
        maxerr_ = std::move(other.maxerr_);
        maxiter_ = std::move(other.maxiter_);
        zval_ = std::move(other.zval_);
        zmid_ = std::move(other.zmid_);
        znew_ = std::move(other.znew_);
        dzdt_ = std::move(other.dzdt_);

        return *this;
    }

    /*
     * Ode functions.
     */
    template<typename Deriv>
    void init(Deriv deriv,
               Vector<T> & core_restrict(x),
               Vector<T> & core_restrict(dxdt),
               T dt);
    template<typename Deriv>
    bool step(Deriv deriv,
               Vector<T> & core_restrict(x),
               Vector<T> & core_restrict(dxdt),
               T dt);
};

/**
 * OdeGauss<T>::init
 * @brief Initialize the solver internal state.
 */
template<typename T, bool IsPar>
template<typename Deriv>
void OdeGauss<T,IsPar>::init(Deriv deriv,
                              Vector<T> & core_restrict(x),
                              Vector<T> & core_restrict(dxdt),
                              T dt)
{
    core_assert(x.n1() == dxdt.n1(), "invalid dimensions");

    /*
     * f(t) = d(x(t))/dt
     */
    deriv(x, dxdt);
}

/**
 * OdeGauss<T>::step
 * @brief Integration step using explicit Euler algorithm.
 */
template<typename T, bool IsPar>
template<typename Deriv>
bool OdeGauss<T,IsPar>::step(Deriv deriv,
                              Vector<T> & core_restrict(x),
                              Vector<T> & core_restrict(dxdt),
                              T dt)
{
    core_assert(x.n1() == dxdt.n1(), "invalid dimensions");

    /*
     * Initialize the half step vector zval
     */
    core_pragma_omp(parallel for if(IsPar) default(none) schedule(static))
    for (size_t i = 0; i < neq_; i++) {
        zval_(i) = 0.0;
    }

    /*
     * Solve the implicit integration step
     * using a fixed point algorithm.
     */
    double err = std::numeric_limits<T>::max();
    size_t iter = 0;
    while (err > maxerr_ && ++iter < maxiter_) {
        /*
         * Compute time derivative at zmid = (x(t+dt) + x(t)) / 2
         */
        core_pragma_omp(parallel for if(IsPar) default(none) \
            shared(x) schedule(static))
        for (size_t i = 0; i < neq_; i++) {
            zmid_(i) = zval_(i) + x(i);
        }

        deriv(zmid_, dzdt_);

        /*
         * Update iterate value znew = 0.5 * dt * dzdt
         */
        core_pragma_omp(parallel for if(IsPar) default(none) \
            shared(dt) schedule(static))
        for (size_t i = 0; i < neq_; i++) {
            znew_(i) = 0.5 * dt * dzdt_(i);
        }

        /*
         * Check convergence and loop
         */
        err = 0.0;
        core_pragma_omp(parallel if(IsPar) default(none) shared(err))
        {
            double thr_err = 0.0;

            core_pragma_omp(for schedule(static))
            for (size_t i = 0; i < neq_; ++i) {
                thr_err += atto::math::fabs<T>(znew_(i) - zval_(i));
                zval_(i) = znew_(i);
            }

            core_pragma_omp(critical)
            {
                err += thr_err;
            } /* omp critical */
        } /* omp parallel */
    }


    /*
     * Integrate the state and update time derivatives
     *
     * x(t+dt) = x(t) + 2.0 * z
     * f(t+dt) = d(x(t+dt))/dt
     */
    core_pragma_omp(parallel for if(IsPar) default(none) \
        shared(x) schedule(static))
    for (size_t i = 0; i < neq_; i++) {
        x(i) += 2.0 * zval_(i);
    }

    deriv(x, dxdt);

    /*
     * Check convergence of the Gauss fixed point iteration.
     */
    return (iter < maxiter_);
}

} /* namespace algo */

#endif /* ALGO_NUMERIC_ODE_GAUSS_H_ */
