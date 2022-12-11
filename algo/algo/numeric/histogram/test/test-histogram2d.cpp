/*
 * test-histogram2d.cpp
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
#include "atto/math/math.hpp"
#include "numeric/histogram/histogram.hpp"

/** ---- Test Rng constants  --------------------------------------------------
 */
static const size_t numsamples = 4194304;
static const size_t numbins1d = 2048;
static const size_t numbins2d = 48;
static const size_t numbins3d = 12;

/** ---- Test Histogram2 ------------------------------------------------------
 */
TEST_CASE("Test Histogram2") {
    constexpr double weight = 1.0;
    std::string filename;
    atto::core::FileOut fout;
    atto::core::FileIn fin;

    /*
     * Random Number Generator
     */
    std::string fmtdbl = atto::core::str_write_format<double>();
    std::string fmtu32 = atto::core::str_write_format<uint32_t>();
    std::string fmtu64 = atto::core::str_write_format<uint64_t>();

    atto::math::rng::Kiss rng_kiss(true);
    atto::math::rng::Cmwc rng_cmwc(true);
    atto::math::rng::gauss<double> nrand;
    atto::math::rng::uniform<double> urand;
    atto::math::rng::uniform<int32_t> irand32;
    atto::math::rng::uniform<int64_t> irand64;

    std::printf("check kiss rng engine\n");
    std::printf("urand   ");
    std::printf(fmtdbl.c_str(), urand(rng_kiss));
    std::printf("\n");
    std::printf("nrand   ");
    std::printf(fmtdbl.c_str(), nrand(rng_kiss));
    std::printf("\n");
    std::printf("irand32 ");
    std::printf(fmtu32.c_str(), irand32(rng_kiss, 0, 9));
    std::printf("\n");
    std::printf("irand64 ");
    std::printf(fmtu64.c_str(), irand64(rng_kiss, 0, 9));
    std::printf("\n");

    std::printf("check cmcw rng engine\n");
    std::printf("urand   ");
    std::printf(fmtdbl.c_str(), urand(rng_cmwc));
    std::printf("\n");
    std::printf("nrand   ");
    std::printf(fmtdbl.c_str(), nrand(rng_cmwc));
    std::printf("\n");
    std::printf("irand32 ");
    std::printf(fmtu32.c_str(), irand32(rng_cmwc, 0, 9));
    std::printf("\n");
    std::printf("irand64 ");
    std::printf(fmtu64.c_str(), irand64(rng_cmwc, 0, 9));
    std::printf("\n");

    /** ---- Kist rng distributions --------------------------------------------
     */
    SECTION("Kiss uniform distribution") {
        /*
         * Kiss uniform distribution
         */
        algo::Histogram2 hist_kiss(numbins2d, numbins2d);
        algo::Histogram2 hist_kiss_tmp(numbins2d, numbins2d);

        hist_kiss.set(0.0, 1.0, 0.0, 1.0, true);
        for(size_t n = 0; n < numsamples; ++n) {
            hist_kiss.bin(urand(rng_kiss), urand(rng_kiss), weight);
        }

        /* Write the histogram */
        filename = TestPrefix + std::string("/out.hist2d_kiss_urand");
        fout.open(filename);
        hist_kiss.write(fout, true);
        fout.close();

        /* Read the histogram and compare */
        hist_kiss_tmp = std::move(hist_kiss);
        fin.open(filename);
        hist_kiss.read(fin);
        fin.close();
        REQUIRE((hist_kiss.xbins_ == hist_kiss_tmp.xbins_ &&
                 hist_kiss.ybins_ == hist_kiss_tmp.ybins_ &&
                 hist_kiss.xmin_  == hist_kiss_tmp.xmin_  &&
                 hist_kiss.xmax_  == hist_kiss_tmp.xmax_  &&
                 hist_kiss.ymin_  == hist_kiss_tmp.ymin_  &&
                 hist_kiss.ymax_  == hist_kiss_tmp.ymax_));

        /*
         * Kiss normal distribution
         */
        hist_kiss.set(-10.0, 10.0, -10.0, 10.0, true);
        for(size_t n = 0; n < numsamples; ++n) {
            hist_kiss.bin(nrand(rng_kiss), nrand(rng_kiss), weight);
        }

        /* Write the histogram */
        filename = TestPrefix + std::string("/out.hist2d_kiss_nrand");
        fout.open(filename);
        hist_kiss.write(fout, true);
        fout.close();

        /* Read the histogram and compare */
        hist_kiss_tmp = std::move(hist_kiss);
        fin.open(filename);
        hist_kiss.read(fin);
        fin.close();
        REQUIRE((hist_kiss.xbins_ == hist_kiss_tmp.xbins_ &&
                 hist_kiss.ybins_ == hist_kiss_tmp.ybins_ &&
                 hist_kiss.xmin_  == hist_kiss_tmp.xmin_  &&
                 hist_kiss.xmax_  == hist_kiss_tmp.xmax_  &&
                 hist_kiss.ymin_  == hist_kiss_tmp.ymin_  &&
                 hist_kiss.ymax_  == hist_kiss_tmp.ymax_));
    }

    /** ---- Cmwc rng distributions --------------------------------------------
     */
    SECTION("Cmwc uniform distribution") {
        /*
         * Cmwc uniform distribution
         */
        algo::Histogram2 hist_cmwc(numbins2d, numbins2d);
        algo::Histogram2 hist_cmwc_tmp(numbins2d, numbins2d);

        hist_cmwc.set(0.0, 1.0, 0.0, 1.0, true);
        for(size_t n = 0; n < numsamples; ++n) {
            hist_cmwc.bin(urand(rng_cmwc), urand(rng_cmwc), weight);
        }

        /* Write the histogram */
        filename = TestPrefix + std::string("/out.hist2d_cmwc_urand");
        fout.open(filename);
        hist_cmwc.write(fout, true);
        fout.close();

        /* Read the histogram and compare */
        hist_cmwc_tmp = std::move(hist_cmwc);
        fin.open(filename);
        hist_cmwc.read(fin);
        fin.close();
        REQUIRE((hist_cmwc.xbins_ == hist_cmwc_tmp.xbins_ &&
                 hist_cmwc.ybins_ == hist_cmwc_tmp.ybins_ &&
                 hist_cmwc.xmin_  == hist_cmwc_tmp.xmin_  &&
                 hist_cmwc.xmax_  == hist_cmwc_tmp.xmax_  &&
                 hist_cmwc.ymin_  == hist_cmwc_tmp.ymin_  &&
                 hist_cmwc.ymax_  == hist_cmwc_tmp.ymax_));

        /*
         * Cmwc normal distribution
         */
        hist_cmwc.set(-10.0, 10.0, -10.0, 10.0, true);
        for(size_t n = 0; n < numsamples; ++n) {
            hist_cmwc.bin(nrand(rng_cmwc), nrand(rng_cmwc), weight);
        }

        /* Write the histogram */
        filename = TestPrefix + std::string("/out.hist2d_cmwc_nrand");
        fout.open(filename);
        hist_cmwc.write(fout, true);
        fout.close();


        /* Read the histogram and compare */
        hist_cmwc_tmp = std::move(hist_cmwc);
        fin.open(filename);
        hist_cmwc.read(fin);
        fin.close();
        REQUIRE((hist_cmwc.xbins_ == hist_cmwc_tmp.xbins_ &&
                 hist_cmwc.ybins_ == hist_cmwc_tmp.ybins_ &&
                 hist_cmwc.xmin_  == hist_cmwc_tmp.xmin_  &&
                 hist_cmwc.xmax_  == hist_cmwc_tmp.xmax_  &&
                 hist_cmwc.ymin_  == hist_cmwc_tmp.ymin_  &&
                 hist_cmwc.ymax_  == hist_cmwc_tmp.ymax_));
    }
}
