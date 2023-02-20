#ifndef MATERIAL_H
#define MATERIAL_H

#include "util/ray.h"
#include "util/hitrecord.h"

class Material {
	public:
	virtual bool scatter(const Ray &rIn, const HitRecord &rec, Rgb &attenuation, Ray &scattered) const = 0;
};
#endif