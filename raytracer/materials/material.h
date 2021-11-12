#ifndef MATERIAL_H
#define MATERIAL_H

#include "util/ray.h"
class Material;

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
		auto scatterDirection = rec.normal + Vec3::randomUnitVector();
		// Catch degenerate scatter direction
		if (scatterDirection.nearZero())
			scatterDirection = rec.normal;
		scattered = Ray(rec.p, scatterDirection);
		attenuation = m_albedo;
		return true;
	}
};

class Metal : public Material {
public:
	Rgb m_albedo;
	double m_fuzz;

public:
	Metal(const Rgb &albedo, double fuzz)
		: m_albedo(albedo),
		m_fuzz(fuzz < 1 ? fuzz : 1) {
	}

	bool scatter(const Ray &rIn, const HitRecord &rec, Rgb &attenuation, Ray &scattered) const final {
		Vec3 reflected = Vec3::reflect(Vec3::unitVector(rIn.direction()), rec.normal);
		scattered = Ray(rec.p, reflected + m_fuzz * Vec3::randomInUnitSphere());
		attenuation = m_albedo;
		return (Vec3::dot(scattered.direction(), rec.normal) > 0);
	}

};
#endif