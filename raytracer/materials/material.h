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

class Dielectric : public Material {
public:
	double m_refractiveIndex;

public:
	Dielectric(double refractiveIndex)
		: m_refractiveIndex(refractiveIndex) {}

	bool scatter(const Ray &rIn, const HitRecord &rec, Rgb &attenuation, Ray &scattered) const final {
		attenuation = Rgb(1.0, 1.0, 1.0);
		double refractionRatio = rec.frontFace ? (1 / m_refractiveIndex) : m_refractiveIndex;

		Vec3 unitDirection = Vec3::unitVector(rIn.direction());
		
		// calculate if total internal reflection allowed based on sin0` = n/n` * sin0
		// n/n` = refraction ratio (refractive index ratio of the 2 materials)
		// refractive index of air = ~1.0
		// refractive index of glass = ~1.5
		double cosTheta = std::fmin(Vec3::dot(-unitDirection, rec.normal), 1);
		double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);
		bool cannotRefract = (refractionRatio * sinTheta) > 1.0;
		Vec3 direction;
		if (cannotRefract || (reflectance(cosTheta, refractionRatio) > MathsHelper::randomDouble())) {
			direction = Vec3::reflect(unitDirection, rec.normal);
		} else {
			direction = Vec3::refract(unitDirection, rec.normal, refractionRatio);
		}

		scattered = Ray(rec.p, direction);
		return true;
	}
private:
	static double reflectance(double cosine, double refIndex) {
		// Use Schlick's approximation for reflectance
		auto r0 = (1 - refIndex) / (1 + refIndex);
		r0 *= r0;
		return r0 + (1 - r0) * std::pow((1 - cosine), 5);
	}
};
#endif