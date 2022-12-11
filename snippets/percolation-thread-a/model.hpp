/*
 * model.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef MODEL_H_
#define MODEL_H_

#include <set>
#include <vector>
#include "base.hpp"
#include "lattice.hpp"
#include "unionfind.hpp"

struct Model {
    std::unique_ptr<Lattice> m_lattice;
    std::unique_ptr<UnionFind> m_uf;
    bool m_percolate_x;
    bool m_percolate_y;

    /** @brief Solve the percolation problem over a random lattice. */
    void execute(const double p_site);

     /** @brief Sample the percolation events of the current lattice. */
    void sample(std::array<uint64_t, 3> &percolate) const;

    Model();
    ~Model();
    Model(const Model &) = delete;
    Model &operator=(const Model &) = delete;
};

#endif /* MODEL_H_ */
