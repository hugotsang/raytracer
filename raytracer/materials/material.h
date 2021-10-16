#ifndef MATERIAL_H
#define MATERIAL_H

#include "util/ray.h"

struct HitRecord;

class Material {
public:
	virtual bool scatter(const Ray &rIn, const HitRecord &rec, Rgb &attenuation, Ray &scattered) const = 0;
};

class Lambertian : public Material {
public:
	Rgb m_albedo;
public:
	Lambertian(const Rgb &albedo) : m_albedo(albedo) {}

	bool scatter(const Ray &rIn, const HitRecord &rec, Rgb &attenuation, Ray &scattered) const final {
		auto scatterDirection = rec.normal + randomUnitVector();
		// Catch degenerate scatter direction
		if (scatterDirection.nearZero())
			scatterDirection = rec.normal;
		scattered = Ray(rec.p, scatterDirection);
		attenuation = m_albedo;
		return true;
	}
};
#endif