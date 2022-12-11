/*
 * ode-kutta.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ALGO_NUMERIC_ODE_KUTTA_H_
#define ALGO_NUMERIC_ODE_KUTTA_H_

#include "atto/core/core.hpp"
#include "numeric/array/array.hpp"
#include "numeric/linalg/linalg.hpp"

namespace algo {

/** ---- OdeKutta integration solver -------------------------------------------
 * struct OdeKutta
 * @brief Solve the set of odes
 *
 *  dx
 *  -- = f(x(t))
 *  dt
 *
 * using Runge Kutta's 4th-order integration
 *
 *  x(t+dt) = x(t) + dt*(f0 + 2*f1 + 2*f2 + f3)/6
 *
 * where dt is the time step size and fi, i=0,1,2,3 are the time
 * derivatives at different points within a time step interval
 *
 * f0 = f(x0) = f(x, t)
 * f1 = f(x1) = f(x + f0*dt/2, t + dt/2)
 * f2 = f(x2) = f(x + f1*dt/2, t + dt/2)
 * f3 = f(x3) = f(x + f2*dt, t + dt)
 *
 * The time derivative of x at time t, f(x(t)), is computed from the
 * derivative operator Deriv. By default Deriv is a noop.
 */
template<typename T = double, bool IsPar = true>
struct OdeKutta {
    static_assert(std::is_floating_point<T>::value, "non floating point type");

    /*
     * Ode member variables.
     */
    size_t neq_;        /* system dimension */
    Vector<T> x0_;      /* Runge-Kutta states */
    Vector<T> x1_;
    Vector<T> x2_;
    Vector<T> x3_;
    Vector<T> f0_;      /* Runge-Kutta derivatives */
    Vector<T> f1_;
    Vector<T> f2_;
    Vector<T> f3_;

    /* Constructor/destructor */
    OdeKutta(size_t neq)
        : neq_(neq)
        , x0_(Vector<T>(neq))
        , x1_(Vector<T>(neq))
        , x2_(Vector<T>(neq))
        , x3_(Vector<T>(neq))
        , f0_(Vector<T>(neq))
        , f1_(Vector<T>(neq))
        , f2_(Vector<T>(neq))
        , f3_(Vector<T>(neq)) {}
    ~OdeKutta() = default;

    /* Disable copy constructor/assignment */
    OdeKutta(const OdeKutta &other)
        : neq_(neq_)
        , x0_(other.x0_)
        , x1_(other.x1_)
        , x2_(other.x2_)
        , x3_(other.x3_)
        , f0_(other.f0_)
        , f1_(other.f1_)
        , f2_(other.f2_)
        , f3_(other.f3_) {}

    OdeKutta &operator=(const OdeKutta &other) {
        if (this == &other) {
            return *this;
        }

        neq_ = neq_;
        x0_ = other.x0_;
        x1_ = other.x1_;
        x2_ = other.x2_;
        x3_ = other.x3_;
        f0_ = other.f0_;
        f1_ = other.f1_;
        f2_ = other.f2_;
        f3_ = other.f3_;

        return *this;
    }

    /* Move constructor/assignment */
    OdeKutta(OdeKutta &&other)
        : neq_(std::move(neq_))
        , x0_(std::move(other.x0_))
        , x1_(std::move(other.x1_))
        , x2_(std::move(other.x2_))
        , x3_(std::move(other.x3_))
        , f0_(std::move(other.f0_))
        , f1_(std::move(other.f1_))
        , f2_(std::move(other.f2_))
        , f3_(std::move(other.f3_)) {}

    OdeKutta &operator=(OdeKutta &&other) {
        if (this == &other) {
            return *this;
        }

        neq_ = std::move(neq_);
        x0_ = std::move(other.x0_);
        x1_ = std::move(other.x1_);
        x2_ = std::move(other.x2_);
        x3_ = std::move(other.x3_);
        f0_ = std::move(other.f0_);
        f1_ = std::move(other.f1_);
        f2_ = std::move(other.f2_);
        f3_ = std::move(other.f3_);

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
 * OdeKutta<T>::init
 * @brief Initialize the solver internal state.
 */
template<typename T, bool IsPar>
template<typename Deriv>
void OdeKutta<T,IsPar>::init(Deriv deriv,
                              Vector<T> & core_restrict(x),
                              Vector<T> & core_restrict(dxdt),
                              T dt)
{
    core_assert(x.n1() == neq_ && dxdt.n1() == neq_, "invalid dimensions");

    /*
     * f(t) = d(x(t))/dt
     */
    deriv(x, dxdt);

    /*
     * x0 = x(t)
     * f0 = d(x0)/dt
     */
    core_pragma_omp(parallel for if(IsPar) default(none) \
        shared(x, dxdt) schedule(static))
    for (size_t i = 0; i < neq_; i++) {
        x0_(i) = x(i);
        f0_(i) = dxdt(i);
    }
}

/**
 * OdeKutta<T>::step
 * @brief Integration step using explicit Runge-Kutta algorithm.
 */
template<typename T, bool IsPar>
template<typename Deriv>
bool OdeKutta<T,IsPar>::step(Deriv deriv,
                              Vector<T> & core_restrict(x),
                              Vector<T> & core_restrict(dxdt),
                              T dt)
{
    core_assert(x.n1() == neq_ && dxdt.n1() == neq_, "invalid dimensions");

    constexpr T c0 = 1.0 / 6.0;
    constexpr T c1 = 2.0 / 6.0;
    constexpr T c2 = 2.0 / 6.0;
    constexpr T c3 = 1.0 / 6.0;

    /*
     * x0 = x(t)
     * f0 = d(x0)/dt
     */
    core_pragma_omp(parallel for if(IsPar) default(none) \
        shared(x, dxdt) schedule(static))
    for (size_t i = 0; i < neq_; i++) {
        x0_(i) = x(i);
        f0_(i) = dxdt(i);
    }

    /*
     * x1 = x(t) + f0 * dt/2
     * f1 = d(x1)/dt
     */
    core_pragma_omp(parallel for if(IsPar) default(none) \
        shared(x, dt) schedule(static))
    for (size_t i = 0; i < neq_; i++) {
        x1_(i) = x(i) + 0.5 * dt * f0_(i);
    }
    deriv(x1_, f1_);

    /*
     * x2 = x(t) + f1 * dt/2
     * f2 = d(x2)/dt
     */
    core_pragma_omp(parallel for if(IsPar) default(none) \
        shared(x, dt) schedule(static))
    for (size_t i = 0; i < neq_; i++) {
        x2_(i) = x(i) + 0.5 * dt * f1_(i);
    }
    deriv(x2_, f2_);

    /*
     * x3 = x(t) + f2 * dt
     * f3 = d(x3)/dt
     */
    core_pragma_omp(parallel for if(IsPar) default(none) \
        shared(x, dt) schedule(static))
    for (size_t i = 0; i < neq_; i++) {
        x3_(i) = x(i) + dt * f2_(i);
    }
    deriv(x3_, f3_);

    /*
     * x(t+dt) = x(t) + dt * (f0 + 2*f1 + 2*f2 + f3) / 6
     * f(t+dt) = d(x(t))/dt
     */
    core_pragma_omp(parallel for if(IsPar) default(none) \
        shared(x, dt) schedule(static))
    for (size_t i = 0; i < neq_; i++) {
        x(i) += dt * (c0*f0_(i) + c1*f1_(i) + c2*f2_(i) + c3*f3_(i));
    }
    deriv(x, dxdt);

    /*
     * Nothing to check.
     */
    return true;
}

} /* namespace algo */

#endif /* ALGO_NUMERIC_ODE_KUTTA_H_ */
