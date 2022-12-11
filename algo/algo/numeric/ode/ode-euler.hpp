/*
 * ode-euler.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ALGO_NUMERIC_ODE_EULER_H_
#define ALGO_NUMERIC_ODE_EULER_H_

#include "atto/core/core.hpp"
#include "numeric/array/array.hpp"
#include "numeric/linalg/linalg.hpp"

namespace algo {

/** ---- OdeEuler integration solver ------------------------------------------
 * struct OdeEuler
 * @brief Solve the set of odes
 *
 * dx
 * -- = f(x(t))
 * dt
 *
 * using explicit Euler's algorithm:
 *
 * x(t+dt) = x(t) + dt*f(x(t))
 *
 * The time derivative of x at time t, f(x(t)), is computed from the derivative
 *  operator Deriv. By default Deriv is a noop.
 */
template<typename T = double, bool IsPar = true>
struct OdeEuler {
    static_assert(std::is_floating_point<T>::value, "non floating point type");

    /* Constructor/destructor */
    OdeEuler(void) {}
    ~OdeEuler() = default;

    /* Ode functions. */
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
 * OdeEuler<T>::init
 * @brief Initialize the solver internal state.
 */
template<typename T, bool IsPar>
template<typename Deriv>
void OdeEuler<T,IsPar>::init(Deriv deriv,
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
 * OdeEuler<T>::step
 * @brief Integration step using explicit Euler algorithm.
 */
template<typename T, bool IsPar>
template<typename Deriv>
bool OdeEuler<T,IsPar>::step(Deriv deriv,
                              Vector<T> & core_restrict(x),
                              Vector<T> & core_restrict(dxdt),
                              T dt)
{
    core_assert(x.n1() == dxdt.n1(), "invalid dimensions");

    /*
     * x(t+dt) = x(t) + dt*f(t)
     */
    core_pragma_omp(parallel for if(IsPar) default(none) \
        shared(x, dxdt, dt) schedule(static))
    for (size_t i = 0; i < x.n1(); i++) {
        x(i) += dt * dxdt(i);
    }

    /*
     * f(t+dt) = d(x(t+dt))/dt
     */
    deriv(x, dxdt);

    /*
     * Nothing to check.
     */
    return true;
}

} /* namespace algo */

#endif /* ALGO_NUMERIC_ODE_EULER_H_ */
