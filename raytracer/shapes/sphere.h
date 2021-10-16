#pragma once

#include "volume.h"
#include "util/vec3.h"

class Sphere : public Volume {
public:
	Point3 m_center;
	double m_radius;
	std::shared_ptr<Material> m_material;

public:
	Sphere()
		: m_center{Point3()},
		  m_radius{0} {}
	Sphere(Point3 center, double radius, std::shared_ptr<Material> material)
		: m_center{center},
		  m_radius{radius},
		  m_material{material} {}

	bool hit(const Ray &r, double tMin, double tMax, HitRecord &rec) const final;

};

bool Sphere::hit(const Ray &r, double tMin, double tMax, HitRecord &rec) const {
	Vec3 oc = r.origin () - m_center;
	auto a = r.direction().lengthSquared();
	auto halfB = Vec3::dot(oc, r.direction());
	auto c = oc.lengthSquared() - m_radius * m_radius;

	auto discriminant = halfB * halfB - a * c;
	if (discriminant < 0)
		return false;
	auto sqrtd = sqrt(discriminant);

	// Find the nearest root that lies in the acceptable range.
	auto root = (-halfB - sqrtd) / a;
	if (root < tMin || tMax < root) {
		root = (-halfB + sqrtd) / a;
		if (root < tMin || tMax < root)
			return false;
	}

	rec.t = root;
	rec.p = r.at(rec.t);
	Vec3 outwardNormal = (rec.p - m_center) / m_radius;
	rec.setFaceNormal(r, outwardNormal);
	rec.material = m_material;

	return true;
}
