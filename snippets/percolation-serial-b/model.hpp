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

struct Model : atto::gl::Drawable {
    /* ---- Model data ----------------------------------------------------- */
    std::unique_ptr<Lattice> m_lattice;
    std::unique_ptr<UnionFind> m_uf;
    std::set<size_t> m_components_xlo;
    std::set<size_t> m_components_xhi;
    std::set<size_t> m_components_ylo;
    std::set<size_t> m_components_yhi;
    bool m_percolate_x;
    bool m_percolate_y;

    /* ---- Model OpenCL data ---------------------------------------------- */
    cl_context m_context = NULL;
    cl_device_id m_device = NULL;
    cl_command_queue m_queue = NULL;
    cl_program m_program = NULL;
    enum {
        NumKernels = 0,
    };
    std::vector<cl_kernel> m_kernels;
    enum {
        NumBuffers = 0,
    };
    std::vector<cl_mem> m_buffers;
    enum {
        NumImages = 0
    };
    std::vector<cl_mem> m_images;

    /* ---- Model OpenGL data ---------------------------------------------- */
    struct GLData {
        GLuint program;                             /* shader program object */
        GLuint texture;                             /* texture data */
        std::unique_ptr<atto::gl::Image> image;     /* texture image */
        std::unique_ptr<atto::gl::Mesh> mesh;       /* texture mesh */
    } m_gl;

    /* ---- Model member functions ----------------------------------------- */
    void handle(const atto::gl::Event &event) override;
    void draw(void *data = nullptr) override;
    void execute(void);

    /** @brief Compute connected components. */
    void compute(void);

    Model();
    ~Model();
    Model(const Model &) = delete;
    Model &operator=(const Model &) = delete;
};

#endif /* MODEL_H_ */
