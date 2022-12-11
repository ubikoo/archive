/*
 * test-ode-kepler.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "Catch2/catch.hpp"
#include "test/common.hpp"
#include "atto/core/core.hpp"
#include "numeric/array/array.hpp"
#include "numeric/ode/ode.hpp"

/** ---- Kepler model ----------------------------------------------------------
 */
struct Kepler {
    double delta_ = 0.0;

    Kepler(double delta) : delta_(delta) {}
    ~Kepler() = default;

    void operator()(algo::Vector<double> & core_restrict(x),
                    algo::Vector<double> & core_restrict(dxdt)) {
        double r = std::sqrt(x(0)*x(0) + x(1)*x(1));
        double inv_r3 = 1.0 / (r*r*r);
        double inv_r5 = 1.0 / (r*r*r*r*r);

        dxdt(0) = x(2);
        dxdt(1) = x(3);
        dxdt(2) = -(inv_r3 + 0.5*3.0*delta_*inv_r5)*x(0);
        dxdt(3) = -(inv_r3 + 0.5*3.0*delta_*inv_r5)*x(1);
    }

    double energy(algo::Vector<double> & core_restrict(x)) {
        double r = std::sqrt(x(0)*x(0) + x(1)*x(1));

        double e_kin = 0.5*(x(2)*x(2) + x(3)*x(3));
        double e_pot = -(1.0/r) - (0.5*delta_/r*r*r);
        return e_kin + e_pot;
    }
};

/** ---- Test Ode Gauss Integrator ---------------------------------------------
 */
void test_ode_kepler_gauss(double dt, size_t nsteps, size_t smpfreq)
{
    /*
     * Model parameters
     */
    const double delta = 0.01;
    Kepler kepler(delta);

    /*
     * System parameters
     */
    const size_t ndim = 4;
    const double e = 0.6;
    algo::Vector<double> x(ndim);          /* x(t) */
    x(0) = 1.0 - e;
    x(1) = 0.0;
    x(2) = 0.0;
    x(3) = std::sqrt((1.0 + e)/(1.0 - e));
    algo::Vector<double> dxdt(ndim);       /* dx(t)/dt */

    /*
     * Solver parameters
     */
    double t = 0.0;
    algo::OdeGauss<double, false> gauss(ndim);
    gauss.init(kepler, x, dxdt, dt);

    /*
     * Integrate system
     */
    atto::core::FileOut fp(TestPrefix + std::string("/out.kepler_gauss"));
    fp.printf("#  t  x(0)  x(1)  x(2)  x(3)  e\n");

    for (size_t i = 0; i < nsteps; ++i) {
        /* sample */
        if (i%smpfreq == 0) {
            fp.printf("%lf %lf %lf %lf %lf %lf\n",
                      t,
                      x(0),
                      x(1),
                      x(2),
                      x(3),
                      kepler.energy(x));
        }

        /* x = x + dxdt*dt */
        gauss.step(kepler, x, dxdt, dt);
        t += dt;
    }

    fp.close();
}




/** ---- Test Ode Runge-Kutta Integrator ---------------------------------------
 */
void test_ode_kepler_kutta(double dt, size_t nsteps, size_t smpfreq)
{
    /*
     * Model parameters
     */
    const double delta = 0.01;
    Kepler kepler(delta);

    /*
     * System parameters
     */
    const size_t ndim = 4;
    const double e = 0.6;
    algo::Vector<double> x(ndim);          /* x(t) */
    x(0) = 1.0 - e;
    x(1) = 0.0;
    x(2) = 0.0;
    x(3) = std::sqrt((1.0 + e)/(1.0 - e));
    algo::Vector<double> dxdt(ndim);       /* dx(t)/dt */

    /*
     * Solver parameters
     */
    double t = 0.0;
    algo::OdeKutta<double, false> kutta(ndim);
    kutta.init(kepler, x, dxdt, dt);

    /*
     * Integrate system
     */
    atto::core::FileOut fp(TestPrefix + std::string("/out.kepler_kutta"));
    fp.printf("#  t  x(0)  x(1)  x(2)  x(3)  e\n");

    for (size_t i = 0; i < nsteps; ++i) {
        /* sample */
        if (i%smpfreq == 0) {
            fp.printf("%lf %lf %lf %lf %lf %lf\n",
                      t,
                      x(0),
                      x(1),
                      x(2),
                      x(3),
                      kepler.energy(x));
        }

        /* x = x + dxdt*dt */
        kutta.step(kepler, x, dxdt, dt);
        t += dt;
    }

    fp.close();
}


/** ---- Test Ode Gear Predictor-Corrector Integrator --------------------------
 */
void test_ode_kepler_gear(double dt, size_t nsteps, size_t smpfreq)
{
    /*
     * Model parameters
     */
    const double delta = 0.01;
    Kepler kepler(delta);

    /*
     * System parameters
     */
    const size_t ndim = 4;
    const double e = 0.6;
    algo::Vector<double> x(ndim);          /* x(t) */
    x(0) = 1.0 - e;
    x(1) = 0.0;
    x(2) = 0.0;
    x(3) = std::sqrt((1.0 + e)/(1.0 - e));
    algo::Vector<double> dxdt(ndim);       /* dx(t)/dt */

    /*
     * Solver parameters
     */
    double t = 0.0;
    algo::OdeGear<double, false> gear(ndim);
    gear.init(kepler, x, dxdt, dt);

    /*
     * Integrate system
     */
    atto::core::FileOut fp(TestPrefix + std::string("/out.kepler_gear"));
    fp.printf("#  t  x(0)  x(1)  x(2)  x(3)  e\n");

    for (size_t i = 0; i < nsteps; ++i) {
        /* sample */
        if (i%smpfreq == 0) {
            fp.printf("%lf %lf %lf %lf %lf %lf\n",
                      t,
                      x(0),
                      x(1),
                      x(2),
                      x(3),
                      kepler.energy(x));
        }

        /* x = x + dxdt*dt */
        gear.step(kepler, x, dxdt, dt);
        t += dt;
    }

    fp.close();
}


/** ---- Test Ode Kepler solver ------------------------------------------------
 */
TEST_CASE("Test Ode Kepler solver") {
    SECTION("OdeGauss") {
        test_ode_kepler_gauss(0.05, 20000, 2);
    }
    SECTION("OdeKutta") {
        test_ode_kepler_kutta(0.05, 20000, 2);
    }
    SECTION("OdeGear") {
        test_ode_kepler_gear( 0.05, 20000, 2);
    }
}
