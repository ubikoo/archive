/*
 * ortho.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef CORE_GEOMETRY_ORTHO_H_
#define CORE_GEOMETRY_ORTHO_H_

#include "vec3.hpp"
#include "mat3.hpp"
#include "algebra.hpp"
#include "io.hpp"

namespace core {
namespace math {

/** ---------------------------------------------------------------------------
 * struct Ortho
 * @brief Ortho is a plain class representing an orthonormal basis.
 */
struct Ortho {
    /* Member variables and access functions. */
    vec3<double> u_;
    vec3<double> v_;
    vec3<double> w_;

    vec3<double> &u(void) { return u_; }
    const vec3<double> &u(void) const { return u_; }

    vec3<double> &v(void) { return v_; }
    const vec3<double> &v(void) const { return v_; }

    vec3<double> &w(void) { return w_; }
    const vec3<double> &w(void) const { return w_; }

    /* Geometry operators. */
    void create_from_u(const vec3<double> &u);
    void create_from_v(const vec3<double> &v);
    void create_from_w(const vec3<double> &w);

    void create_from_uv(const vec3<double> &u, const vec3<double> &v);
    void create_from_vw(const vec3<double> &v, const vec3<double> &w);
    void create_from_wu(const vec3<double> &w, const vec3<double> &u);

    vec3<double> world_to_local(const vec3<double> &a) const;
    vec3<double> local_to_world(const vec3<double> &a) const;

    /* Default onstructor/destructor. */
    Ortho()
        : u_(vec3<double>{1.0, 0.0, 0.0})
        , v_(vec3<double>{0.0, 1.0, 0.0})
        , w_(vec3<double>{0.0, 0.0, 1.0}) {}
    ~Ortho() = default;

    /* Copy constructor/assignment. */
    Ortho(const Ortho &other)
        : u_(other.u_)
        , v_(other.v_)
        , w_(other.w_) {}

    Ortho &operator=(const Ortho &other) {
        u_ = other.u_;
        v_ = other.v_;
        w_ = other.w_;
        return *this;
    }
};


/** ---- Geometry operators --------------------------------------------------
 *
 * Ortho::create_from_u
 * @brief Create an orthonormal basis set given u.
 */
core_inline_
void Ortho::create_from_u(const vec3<double> &a)
{
    constexpr double eps = std::sqrt(std::numeric_limits<double>::epsilon());
    const vec3<double> e1(1.0, 0.0, 0.0);
    const vec3<double> e2(0.0, 1.0, 0.0);

    u_ = normalize(a);              /* u = <a> */
    v_ = cross(u_, e1);             /* v = u x e1 */
    if (norm(v_) < eps) {
        v_ = cross(u_, e2);         /* v = u x e2 */
    }
    v_ = normalize(v_);
    w_ = normalize(cross(u_, v_));  /* w = <u x v> */
}

/**
 * Ortho::create_from_v
 * @brief Create an orthonormal basis set given v.
 */
core_inline_
void Ortho::create_from_v(const vec3<double> &a)
{
    constexpr double eps = std::sqrt(std::numeric_limits<double>::epsilon());
    const vec3<double> e1(1.0, 0.0, 0.0);
    const vec3<double> e2(0.0, 1.0, 0.0);

    v_ = normalize(a);              /* v = <a> */
    w_ = cross(v_, e1);             /* w = v x e1 */
    if (norm(w_) < eps) {
        w_ = cross(v_, e2);         /* w = v x e2 */
    }
    w_ = normalize(w_);
    u_ = normalize(cross(v_, w_));  /* u = <v x w> */
}

/**
 * Ortho::create_from_w
 * @brief Create an orthonormal basis set given w.
 */
core_inline_
void Ortho::create_from_w(const vec3<double> &a)
{
    constexpr double eps = std::sqrt(std::numeric_limits<double>::epsilon());
    const vec3<double> e1(1.0, 0.0, 0.0);
    const vec3<double> e2(0.0, 1.0, 0.0);

    w_ = normalize(a);              /* w = <a> */
    u_ = cross(w_, e1);             /* u = w x e1 */
    if (norm(u_) < eps) {
        u_ = cross(w_, e2);         /* u = w x e2 */
    }
    u_ = normalize(u_);
    v_ = normalize(cross(w_, u_));  /* v = <w x u> */
}

/**
 * Ortho::create_from_uv
 * @brief Create an orthonormal basis set given the basis vectors
 * u and v, orthogonalized using a Gram-Schmidt process.
 */
core_inline_
void Ortho::create_from_uv(const vec3<double> &a, const vec3<double> &b)
{
    u_ = normalize(a);                      /* u = <a> */
    v_ = normalize(b - (u_ * dot(b, u_)));  /* v = <b - (b.u)*u> */
    w_ = normalize(cross(u_, v_));          /* w = u x v */
}

/**
 * Ortho::create_from_vw
 * @brief Create an orthonormal basis set given the basis vectors
 * v and w, orthogonalized using a Gram-Schmidt process.
 */
core_inline_
void Ortho::create_from_vw(const vec3<double> &a, const vec3<double> &b)
{
    v_ = normalize(a);                      /* v = <a> */
    w_ = normalize(b - (v_ * dot(b, v_)));  /* w = <b - (b.v)*v> */
    u_ = normalize(cross(v_, w_));          /* u = <v x w> */
}

/**
 * Ortho::create_from_wu
 * @brief Create an orthonormal basis set given the basis vectors
 * w and u, orthogonalized using a Gram-Schmidt process.
 */
core_inline_
void Ortho::create_from_wu(const vec3<double> &a, const vec3<double> &b)
{
    w_ = normalize(a);                      /* v = <a> */
    u_ = normalize(b - (w_ * dot(b, w_)));  /* u = <b - (b.w)*w> */
    v_ = normalize(cross(w_, u_));          /* v = <w x u> */
}

/**
 * Ortho::world_to_local
 * @brief Project a vector coordinates from world to local
 * reference frame defined by the orthonormal basis set.
 */
core_inline_
vec3<double> Ortho::world_to_local(const vec3<double> &a) const
{
    const mat3<double> M(
        u_(0), u_(1), u_(2),
        v_(0), v_(1), v_(2),
        w_(0), w_(1), w_(2));
    return multiply(M, a);
}

/**
 * Ortho::local_to_world
 * @brief Project a vector coordinates from local to world
 * reference frame defined by the orthonormal basis set.
 */
core_inline_
vec3<double> Ortho::local_to_world(const vec3<double> &a) const
{
    const mat3<double> M(
        u_(0), v_(0), w_(0),
        u_(1), v_(1), w_(1),
        u_(2), v_(2), w_(2));
    return multiply(M, a);
}


/** ---------------------------------------------------------------------------
 * @brief Write a string representation of the orthonormal vectors
 * using the appropriate format specifier.
 */
core_inline_
std::string to_string(const Ortho &o, const char *format)
{
    std::ostringstream ss;
    ss << to_string(o.u(), format);
    ss << to_string(o.v(), format);
    ss << to_string(o.w(), format);
    return ss.str();
}

} /* namespace math */
} /* namespace core */

/*
 * C linkage declarations
 */
#undef  __BEGIN_DECLS
#undef  __END_DECLS
#ifdef  __cplusplus
#define __BEGIN_DECLS extern "C" {
#define __END_DECLS }
#else
#define __BEGIN_DECLS   /* empty */
#define __END_DECLS     /* empty */
#endif
__BEGIN_DECLS
__END_DECLS

#endif /* CORE_GEOMETRY_ORTHO_H_ */
