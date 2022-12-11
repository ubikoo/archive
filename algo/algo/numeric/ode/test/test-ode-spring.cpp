/*
 * test-ode-spring.cpp
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

/** ---- Harmonic spring model -------------------------------------------------
 */
struct Spring {
    double kappa_ = 0.0;
    double alpha_ = 0.0;

    Spring(double kappa, double alpha)
        : kappa_(kappa)
        , alpha_(alpha) {}
    ~Spring() = default;

    void operator()(algo::Vector<double> & core_restrict(x),
                     algo::Vector<double> & core_restrict(dxdt)) {
        dxdt(0) = x(1);
        dxdt(1) = -kappa_*x(0) - 2.0*alpha_*x(1);
    }

    double energy(algo::Vector<double> & core_restrict(x)) {
        return (0.5*kappa_*x(0)*x(0) + 0.5*x(1)*x(1));
    }

    void compute(double t, double v0,
                  algo::Vector<double> & core_restrict(x)) {

        if (std::fabs(alpha_) > 0.0 && alpha_*alpha_ > kappa_) {
            /*
             * Overdamped system
             */
            double w1 = (-std::sqrt(2.0) + 1.0)*std::sqrt(kappa_);
            double w2 = (-std::sqrt(2.0) - 1.0)*std::sqrt(kappa_);

            x(0) = v0 * (std::exp(w1*t) - std::exp(w2*t)) / (w1 - w2);
            x(1) = v0 * (w1*std::exp(w1*t) - w2*std::exp(w2*t)) / (w1 - w2);
        } else if (std::fabs(alpha_) > 0.0 && alpha_*alpha_ < kappa_) {
            /*
             * Underdamped system
             */
            double mu = std::sqrt(kappa_ / 2.0);

            x(0) = v0 * std::exp(-mu*t) * std::sin(mu*t) / mu;
            x(1) = v0 * std::exp(-mu*t) * (std::cos(mu*t)-std::sin(mu*t));
        } else {
            /*
             * Oscillatory system
             */
            double mu = std::sqrt(kappa_);

            x(0) = v0 * std::sin(mu*t) / mu;
            x(1) = v0 * std::cos(mu*t);
        }
    }
};


/** ---- Test Ode Euler Integrator ---------------------------------------------
 */
void test_ode_spring_euler(double dt, size_t nsteps, size_t smpfreq)
{
    atto::core::FileOut fp;

    /* Model parameters */
    const double kappa = 2.0*M_PI*0.5;

    /* Solver parameters */
    algo::OdeEuler<double, false> euler;

    /* System parameters */
    double t = 0.0;
    const double x0 = 0.0;
    const double v0 = 1.0;

    algo::Vector<double> x(2);         /* x(t) */
    algo::Vector<double> dxdt(2);      /* dx(t)/dt */
    algo::Vector<double> xref(2);      /* reference x(t) */

    /** ---- Damped non oscillatory spring -------------------------------------
     */
    t = 0.0;
    x(0) = x0;                                          /* x(t=0) */
    x(1) = v0;                                          /* v(t=0) */
    algo::copy_vector<double, false>(x, xref);          /* xref(t=0) */

    Spring spring1(kappa, std::sqrt(2.0*kappa));
    euler.init(spring1, x, dxdt, dt);

    fp.open(TestPrefix + std::string("/out.euler1"));
    fp.printf("#  t  x(0)  x(1)  e1  x  v  e2\n");

    for (size_t i = 0; i < nsteps; ++i) {
        /* sample */
        if (i%smpfreq == 0) {
            fp.printf("%lf %lf %lf %lf %lf %lf %lf\n",
                      t,
                      x(0),
                      x(1),
                      spring1.energy(x),
                      xref(0),
                      xref(1),
                      spring1.energy(xref));
        }

        /* x = x + dxdt*dt */
        euler.step(spring1, x, dxdt, dt);
        t += dt;

        /* xref */
        spring1.compute(t, v0, xref);
    }

    fp.close();

    /** ---- Damped oscillatory spring -----------------------------------------
     */
    t = 0.0;
    x(0) = x0;                                          /* x(t=0) */
    x(1) = v0;                                          /* v(t=0) */
    algo::copy_vector<double, false>(x, xref);          /* xref(t=0) */

    Spring spring2(kappa, std::sqrt(0.5*kappa));
    euler.init(spring2, x, dxdt, dt);

    fp.open(TestPrefix + std::string("/out.euler2"));
    fp.printf("#  t  x(0)  x(1)  e1  x  v  e2\n");

    for (size_t i = 0; i < nsteps; ++i) {
        /* sample */
        if (i%smpfreq == 0) {
            fp.printf("%lf %lf %lf %lf %lf %lf %lf\n",
                      t,
                      x(0),
                      x(1),
                      spring2.energy(x),
                      xref(0),
                      xref(1),
                      spring2.energy(xref));
        }

        /* x = x + dxdt*dt */
        euler.step(spring2, x, dxdt, dt);
        t += dt;

        /* xref */
        spring2.compute(t, v0, xref);
    }

    fp.close();

    /** ---- Non damped oscillatory spring -------------------------------------
     */
    t = 0.0;
    x(0) = x0;                                          /* x(t=0) */
    x(1) = v0;                                          /* v(t=0) */
    algo::copy_vector<double, false>(x, xref);          /* xref(t=0) */

    Spring spring3(kappa, 0.0*kappa);
    euler.init(spring3, x, dxdt, dt);

    fp.open(TestPrefix + std::string("/out.euler3"));
    fp.printf("#  t  x(0)  x(1)  e1  x  v  e2\n");

    for (size_t i = 0; i < nsteps; ++i) {
        /* sample */
        if (i%smpfreq == 0) {
            fp.printf("%lf %lf %lf %lf %lf %lf %lf\n",
                      t,
                      x(0),
                      x(1),
                      spring3.energy(x),
                      xref(0),
                      xref(1),
                      spring3.energy(xref));
        }

        /* x = x + dxdt*dt */
        euler.step(spring3, x, dxdt, dt);
        t += dt;

        /* xref */
        spring3.compute(t, v0, xref);
    }

    fp.close();
}


/** ---- Test Ode Gauss Integrator ---------------------------------------------
 */
void test_ode_spring_gauss(double dt, size_t nsteps, size_t smpfreq)
{
    atto::core::FileOut fp;

    /* Model parameters */
    const double kappa = 2.0*M_PI*0.5;

    /* Solver parameters */
    algo::OdeGauss<double, false> gauss(2, 1.0E-12, 20);

    /* System parameters */
    double t = 0.0;
    const double x0 = 0.0;
    const double v0 = 1.0;

    algo::Vector<double> x(2);         /* x(t) */
    algo::Vector<double> dxdt(2);      /* dx(t)/dt */
    algo::Vector<double> xref(2);      /* reference x(t) */

    /** ---- Damped non oscillatory spring -------------------------------------
     */
    t = 0.0;
    x(0) = x0;                                          /* x(t=0) */
    x(1) = v0;                                          /* v(t=0) */
    algo::copy_vector<double, false>(x, xref);          /* xref(t=0) */

    Spring spring1(kappa, std::sqrt(2.0*kappa));
    gauss.init(spring1, x, dxdt, dt);

    fp.open(TestPrefix + std::string("/out.gauss1"));
    fp.printf("#  t  x(0)  x(1)  e1  x  v  e2\n");

    for (size_t i = 0; i < nsteps; ++i) {
        /* sample */
        if (i%smpfreq == 0) {
            fp.printf("%lf %lf %lf %lf %lf %lf %lf\n",
                      t,
                      x(0),
                      x(1),
                      spring1.energy(x),
                      xref(0),
                      xref(1),
                      spring1.energy(xref));
        }

        /* x = x + dxdt*dt */
        gauss.step(spring1, x, dxdt, dt);
        t += dt;

        /* xref */
        spring1.compute(t, v0, xref);
    }

    fp.close();

    /** ---- Damped oscillatory spring -----------------------------------------
     */
    t = 0.0;
    x(0) = x0;                                          /* x(t=0) */
    x(1) = v0;                                          /* v(t=0) */
    algo::copy_vector<double, false>(x, xref);          /* xref(t=0) */

    Spring spring2(kappa, std::sqrt(0.5*kappa));
    gauss.init(spring2, x, dxdt, dt);

    fp.open(TestPrefix + std::string("/out.gauss2"));
    fp.printf("#  t  x(0)  x(1)  e1  x  v  e2\n");

    for (size_t i = 0; i < nsteps; ++i) {
        /* sample */
        if (i%smpfreq == 0) {
            fp.printf("%lf %lf %lf %lf %lf %lf %lf\n",
                      t,
                      x(0),
                      x(1),
                      spring2.energy(x),
                      xref(0),
                      xref(1),
                      spring2.energy(xref));
        }

        /* x = x + dxdt*dt */
        gauss.step(spring2, x, dxdt, dt);
        t += dt;

        /* xref */
        spring2.compute(t, v0, xref);
    }

    fp.close();



    /** ---- Non damped oscillatory spring -------------------------------------
     */
    t = 0.0;
    x(0) = x0;                                          /* x(t=0) */
    x(1) = v0;                                          /* v(t=0) */
    algo::copy_vector<double, false>(x, xref);          /* xref(t=0) */

    Spring spring3(kappa, 0.0*kappa);
    gauss.init(spring3, x, dxdt, dt);

    fp.open(TestPrefix + std::string("/out.gauss3"));
    fp.printf("#  t  x(0)  x(1)  e1  x  v  e2\n");

    for (size_t i = 0; i < nsteps; ++i) {
        /* sample */
        if (i%smpfreq == 0) {
            fp.printf("%lf %lf %lf %lf %lf %lf %lf\n",
                      t,
                      x(0),
                      x(1),
                      spring3.energy(x),
                      xref(0),
                      xref(1),
                      spring3.energy(xref));
        }

        /* x = x + dxdt*dt */
        gauss.step(spring3, x, dxdt, dt);
        t += dt;

        /* xref */
        spring3.compute(t, v0, xref);
    }

    fp.close();
}




/** ---- Test Ode Runge-Kutta Integrator ---------------------------------------
 */
void test_ode_spring_kutta(double dt, size_t nsteps, size_t smpfreq)
{
    atto::core::FileOut fp;

    /* Model parameters */
    const double kappa = 2.0*M_PI*0.5;

    /* Solver parameters */
    algo::OdeKutta<double, false> kutta(2);

    /* System parameters */
    double t = 0.0;
    const double x0 = 0.0;
    const double v0 = 1.0;

    algo::Vector<double> x(2);        /* x(t) */
    algo::Vector<double> dxdt(2);     /* dx(t)/dt */
    algo::Vector<double> xref(2);     /* reference x(t) */



    /** ---- Damped non oscillatory spring -------------------------------------
     */
    t = 0.0;
    x(0) = x0;                                          /* x(t=0) */
    x(1) = v0;                                          /* v(t=0) */
    algo::copy_vector<double, false>(x, xref);          /* xref(t=0) */

    Spring spring1(kappa, std::sqrt(2.0*kappa));
    kutta.init(spring1, x, dxdt, dt);

    fp.open(TestPrefix + std::string("/out.kutta1"));
    fp.printf("#  t  x(0)  x(1)  e1  x  v  e2\n");

    for (size_t i = 0; i < nsteps; ++i) {
        /* sample */
        if (i%smpfreq == 0) {
            fp.printf("%lf %lf %lf %lf %lf %lf %lf\n",
                      t,
                      x(0),
                      x(1),
                      spring1.energy(x),
                      xref(0),
                      xref(1),
                      spring1.energy(xref));
        }

        /* x = x + dxdt*dt */
        kutta.step(spring1, x, dxdt, dt);
        t += dt;

        /* xref */
        spring1.compute(t, v0, xref);
    }

    fp.close();



    /** ---- Damped oscillatory spring -----------------------------------------
     */
    t = 0.0;
    x(0) = x0;                                          /* x(t=0) */
    x(1) = v0;                                          /* v(t=0) */
    algo::copy_vector<double, false>(x, xref);          /* xref(t=0) */

    Spring spring2(kappa, std::sqrt(0.5*kappa));
    kutta.init(spring2, x, dxdt, dt);

    fp.open(TestPrefix + std::string("/out.kutta2"));
    fp.printf("#  t  x(0)  x(1)  e1  x  v  e2\n");

    for (size_t i = 0; i < nsteps; ++i) {
        /* sample */
        if (i%smpfreq == 0) {
            fp.printf("%lf %lf %lf %lf %lf %lf %lf\n",
                      t,
                      x(0),
                      x(1),
                      spring2.energy(x),
                      xref(0),
                      xref(1),
                      spring2.energy(xref));
        }

        /* x = x + dxdt*dt */
        kutta.step(spring2, x, dxdt, dt);
        t += dt;

        /* xref */
        spring2.compute(t, v0, xref);
    }

    fp.close();



    /** ---- Non damped oscillatory spring -------------------------------------
     */
    t = 0.0;
    x(0) = x0;                                          /* x(t=0) */
    x(1) = v0;                                          /* v(t=0) */
    algo::copy_vector<double, false>(x, xref);          /* xref(t=0) */

    Spring spring3(kappa, 0.0*kappa);
    kutta.init(spring3, x, dxdt, dt);

    fp.open(TestPrefix + std::string("/out.kutta3"));
    fp.printf("#  t  x(0)  x(1)  e1  x  v  e2\n");

    for (size_t i = 0; i < nsteps; ++i) {
        /* sample */
        if (i%smpfreq == 0) {
            fp.printf("%lf %lf %lf %lf %lf %lf %lf\n",
                      t,
                      x(0),
                      x(1),
                      spring3.energy(x),
                      xref(0),
                      xref(1),
                      spring3.energy(xref));
        }

        /* x = x + dxdt*dt */
        kutta.step(spring3, x, dxdt, dt);
        t += dt;

        /* xref */
        spring3.compute(t, v0, xref);
    }

    fp.close();
}




/** ---- Test Ode Gear Predictor-Corrector Integrator --------------------------
 */
void test_ode_spring_gear(double dt, size_t nsteps, size_t smpfreq)
{
    atto::core::FileOut fp;

    /* Model parameters */
    const double kappa = 2.0*M_PI*0.5;

    /* Solver parameters */
    algo::OdeGear<double, false> gear(2);

    /* System parameters */
    double t = 0.0;
    const double x0 = 0.0;
    const double v0 = 1.0;

    algo::Vector<double> x(2);        /* x(t) */
    algo::Vector<double> dxdt(2);     /* dx(t)/dt */
    algo::Vector<double> xref(2);     /* reference x(t) */



    /** ---- Damped non oscillatory spring -------------------------------------
     */
    t = 0.0;
    x(0) = x0;                                          /* x(t=0) */
    x(1) = v0;                                          /* v(t=0) */
    algo::copy_vector<double, false>(x, xref);          /* xref(t=0) */

    Spring spring1(kappa, std::sqrt(2.0*kappa));
    gear.init(spring1, x, dxdt, dt);

    fp.open(TestPrefix + std::string("/out.gear1"));
    fp.printf("#  t  x(0)  x(1)  e1  x  v  e2\n");

    for (size_t i = 0; i < nsteps; ++i) {
        /* sample */
        if (i%smpfreq == 0) {
            fp.printf("%lf %lf %lf %lf %lf %lf %lf\n",
                      t,
                      x(0),
                      x(1),
                      spring1.energy(x),
                      xref(0),
                      xref(1),
                      spring1.energy(xref));
        }

        /* x = x + dxdt*dt */
        gear.step(spring1, x, dxdt, dt);
        t += dt;

        /* xref */
        spring1.compute(t, v0, xref);
    }

    fp.close();



    /** ---- Damped oscillatory spring -----------------------------------------
     */
    t = 0.0;
    x(0) = x0;                                          /* x(t=0) */
    x(1) = v0;                                          /* v(t=0) */
    algo::copy_vector<double, false>(x, xref);          /* xref(t=0) */

    Spring spring2(kappa, std::sqrt(0.5*kappa));
    gear.init(spring2, x, dxdt, dt);

    fp.open(TestPrefix + std::string("/out.gear2"));
    fp.printf("#  t  x(0)  x(1)  e1  x  v  e2\n");

    for (size_t i = 0; i < nsteps; ++i) {
        /* sample */
        if (i%smpfreq == 0) {
            fp.printf("%lf %lf %lf %lf %lf %lf %lf\n",
                      t,
                      x(0),
                      x(1),
                      spring2.energy(x),
                      xref(0),
                      xref(1),
                      spring2.energy(xref));
        }

        /* x = x + dxdt*dt */
        gear.step(spring2, x, dxdt, dt);
        t += dt;

        /* xref */
        spring2.compute(t, v0, xref);
    }

    fp.close();



    /** ---- Non damped oscillatory spring -------------------------------------
     */
    t = 0.0;
    x(0) = x0;                                          /* x(t=0) */
    x(1) = v0;                                          /* v(t=0) */
    algo::copy_vector<double, false>(x, xref);          /* xref(t=0) */

    Spring spring3(kappa, 0.0*kappa);
    gear.init(spring3, x, dxdt, dt);

    fp.open(TestPrefix + std::string("/out.gear3"));
    fp.printf("#  t  x(0)  x(1)  e1  x  v  e2\n");

    for (size_t i = 0; i < nsteps; ++i) {
        /* sample */
        if (i%smpfreq == 0) {
            fp.printf("%lf %lf %lf %lf %lf %lf %lf\n",
                      t,
                      x(0),
                      x(1),
                      spring3.energy(x),
                      xref(0),
                      xref(1),
                      spring3.energy(xref));
        }

        /* x = x + dxdt*dt */
        gear.step(spring3, x, dxdt, dt);
        t += dt;

        /* xref */
        spring3.compute(t, v0, xref);
    }

    fp.close();
}



/** ---- Test Ode spring solver ------------------------------------------------
 */
TEST_CASE("Test Ode spring solver") {
    SECTION("OdeEuler") {
        test_ode_spring_euler(0.00001, 10000000, 1000);
    }
    SECTION("OdeGauss") {
        test_ode_spring_gauss(0.001, 100000, 10);
    }
    SECTION("OdeKutta") {
        test_ode_spring_kutta(0.001, 100000, 10);
    }
    SECTION("OdeGear") {
        test_ode_spring_gear( 0.001, 100000, 10);
    }
}
