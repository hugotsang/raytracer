#pragma once
#include "util/hitrecord.h"

class Volume {
public:
	virtual bool hit(const Ray &r, double tMin, double tMax, HitRecord &rec) const = 0;
};