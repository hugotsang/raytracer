#pragma once

#include "vec3.h"

class Ray {
public:
    Point3 m_orig;
    Vec3 m_dir;

public:
    Ray() {}
    Ray(const Point3& origin, const Vec3& direction)
        : m_orig(origin),
        m_dir(direction) {}

    Point3 origin() const { return m_orig; }
    Vec3 direction() const { return m_dir; }

    Point3 at(double t) const {
        return m_orig + t * m_dir;
    }
};