#pragma once
#include "materials/material.h"

struct HitRecord {
	Point3 p;
	Vec3 normal;
    std::shared_ptr<Material> material;
	double t;
    bool frontFace;

    inline void setFaceNormal(const Ray &r, const Vec3 &outwardNormal) {
        frontFace = Vec3::dot(r.direction(), outwardNormal) < 0;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};

class Volume {
public:
	virtual bool hit(const Ray &r, double tMin, double tMax, HitRecord &rec) const = 0;
};