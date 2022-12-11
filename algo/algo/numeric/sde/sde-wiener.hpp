/*
 * sde-wiener.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ALGO_NUMERIC_SDE_WIENER_H_
#define ALGO_NUMERIC_SDE_WIENER_H_

#include "atto/core/core.hpp"
#include "atto/math/math.hpp"
#include "numeric/array/array.hpp"
#include "numeric/linalg/linalg.hpp"
#include "numeric/random.hpp"

namespace algo {

/** ---- Sde Wiener process generator ------------------------------------------
 * struct SdeWiener
 *
 * Compute a Wiener stochastic process
 *
 * dw = sqrt(dt) * N(0,1)
 *
 * where N(0,1) is a Gaussian distribution with zero mean and unit variance.
 * The Wiener generator is an array of random number generators that run in
 * parallel on each thread available.
 */
template<typename T = double,
         typename R = atto::math::rng::Kiss,
         bool IsPar = true>
struct SdeWiener {
    static_assert(std::is_floating_point<T>::value, "non floating point type");

    /* SdeWiener member variables. */
    std::vector<R> engine_;                         /* rng engines */
    std::vector<atto::math::rng::gauss<T>> random_; /* rng distributions */

    /* Constructor/destructor */
    SdeWiener() {
        /* Create a set of random number generators */
        core_pragma_omp(parallel default(none) shared(engine_, random_))
        {
            core_pragma_omp(master)
            {
                int num_threads = omp_get_num_threads();
                engine_.resize(num_threads);
                for (auto &e : engine_) { e.init(); }
                random_.resize(num_threads);
            } /* omp master */
        } /* omp parallel */
    }
    ~SdeWiener() = default;

    /* Copy constructor/assignment */
    SdeWiener(const SdeWiener &other) = delete;
    SdeWiener &operator=(const SdeWiener &other) = delete;

    /* SdeWiener functions */
    void operator()(T dt, Vector<T> & core_restrict(dw));
};

/**
 * SdeWiener<T>::operator()
 */
template<typename T, typename R, bool IsPar>
void SdeWiener<T,R,IsPar>::operator()(T dt, Vector<T> & core_restrict(dw))
{
    /*
     * dw = sqrt(dt) * N(0,1)
     */
    core_pragma_omp(parallel if(IsPar) default(none) shared(dt, dw))
    {
        size_t tid = omp_get_thread_num();
        T dt2 = std::sqrt(dt);

        core_pragma_omp(for schedule(static))
        for (size_t i = 0; i < dw.n1(); i++) {
            dw(i) = dt2 * random_[tid](engine_[tid]);
        }
    } /* omp parallel */
}

} /* namespace algo */

#endif /* ALGO_NUMERIC_SDE_WIENER_H_ */
