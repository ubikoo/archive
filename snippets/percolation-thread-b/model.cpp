/*
 * model.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "model.hpp"
using namespace atto;

/** ---------------------------------------------------------------------------
 * Model::Model
 * @brief Create OpenCL context and associated objects.
 */
Model::Model(void)
{
    /*
     * Setup Model data.
     */
    {
        m_lattice = std::make_unique<Lattice>(Params::n_sites);
        m_uf = std::make_unique<UnionFind>(Params::n_sites * Params::n_sites);
    }
}

/**
 * Model::~Model
 * @brief Destroy the OpenCL context and associated objects.
 */
Model::~Model()
{}

/** ---------------------------------------------------------------------------
 * Model::execute
 * @brief Solve the percolation problem over a random lattice.
 */
void Model::execute(const double p_site)
{
    /* Generate a new lattice state */
    m_lattice->generate(p_site);

    /* Compute connected components using a unionfind data structure */
    m_uf->clear();
    for (int32_t x = 0; x < Params::n_sites; ++x) {
        for (int32_t y = 0; y < Params::n_sites; ++y) {
            if (m_lattice->is_open(x, y)) {
                int32_t ix_0 = m_lattice->index(x, y);

                if (x > 0 && m_lattice->is_open(x-1, y)) {
                    int32_t ix_1 = m_lattice->index(x-1, y);
                    m_uf->join(ix_0, ix_1);
                }

                if (y > 0 && m_lattice->is_open(x, y-1)) {
                    int32_t ix_1 = m_lattice->index(x, y-1);
                    m_uf->join(ix_0, ix_1);
                }
            }
        }
    }

    /* Does the lattice percolate in the x-direction? */
    m_percolate_x = false;
    {
        std::set<size_t> components_xlo;
        for (int32_t y = 0; y < Params::n_sites; ++y) {
            if (m_lattice->is_open(0, y)) {
                int32_t ix = m_lattice->index(0, y);
                components_xlo.insert(m_uf->find(ix));
            }
        }

        std::set<size_t> components_xhi;
        for (int32_t y = 0; y < Params::n_sites; ++y) {
            if (m_lattice->is_open(Params::n_sites-1, y)) {
                int32_t ix = m_lattice->index(Params::n_sites-1, y);
                components_xhi.insert(m_uf->find(ix));
            }
        }

        for (auto &it : components_xlo) {
            if (components_xhi.find(it) != components_xhi.end()) {
                m_percolate_x = true;
                break;
            }
        }
    }

    /* Does the lattice percolate in the y-direction? */
    m_percolate_y = false;
    {
        std::set<size_t> components_ylo;
        for (int32_t x = 0; x < Params::n_sites; ++x) {
            if (m_lattice->is_open(x, 0)) {
                int32_t ix = m_lattice->index(x, 0);
                components_ylo.insert(m_uf->find(ix));
            }
        }

        std::set<size_t> components_yhi;
        for (int32_t x = 0; x < Params::n_sites; ++x) {
            if (m_lattice->is_open(x, Params::n_sites-1)) {
                int32_t ix = m_lattice->index(x, Params::n_sites-1);
                components_yhi.insert(m_uf->find(ix));
            }
        }

        for (auto &it : components_ylo) {
            if (components_yhi.find(it) != components_yhi.end()) {
                m_percolate_y = true;
                break;
            }
        }
    }
}

/**
 * Model::sample
 * @brief Sample the percolation events of the current lattice.
 */
void Model::sample(std::array<uint64_t, 3> &percolate) const
{
    uint8_t cx = m_percolate_x ? 1 : 0;
    uint8_t cy = m_percolate_y ? 1 : 0;
    percolate[0] += cx;
    percolate[1] += cy;
    percolate[2] += cx * cy;
}

