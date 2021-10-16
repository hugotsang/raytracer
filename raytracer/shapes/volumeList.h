#pragma once

#include <memory>
#include <vector>

#include "volume.h"

class VolumeList : public Volume {
public:
	std::vector<std::shared_ptr<Volume>> m_objects;

public:
	VolumeList() {}
	VolumeList(std::shared_ptr<Volume> object) {
		add(object);
	}

	void clear() {
		m_objects.clear();
	}

	void add(std::shared_ptr<Volume> object) {
		m_objects.push_back(object);
	}

	bool hit(const Ray &r, double tMin, double tMax, HitRecord &rec) const final;
};

bool VolumeList::hit(const Ray &r, double tMin, double tMax, HitRecord &rec) const {
	HitRecord tempRec;
	bool isHit = false;
	auto closestSoFar = tMax;

	for (const auto &object : m_objects) {
		if (object->hit(r, tMin, closestSoFar, tempRec)) {
			isHit = true;
			closestSoFar = tempRec.t;
			rec = tempRec;
		}
	}

	return isHit;
}
