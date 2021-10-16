#pragma once

#include "util/mathsHelper.h"

class Camera {
public:
	Camera() {
		auto aspectRatio = 16.0 / 9.0;
		auto viewportHeight = 2.0;
		auto viewportWidth = aspectRatio * viewportHeight;
		auto focalLength = 1.0;

		m_origin = Point3(0, 0, 0);
		m_horizontal = Vec3(viewportWidth, 0.0, 0.0);
		m_vertical = Vec3(0.0, viewportHeight, 0.0);
		m_lowerLeftCorner = m_origin - (m_horizontal / 2) - (m_vertical / 2) - Vec3(0, 0, focalLength);
	}

	Ray getRay(double u, double v) const {
		return Ray(m_origin, m_lowerLeftCorner + (u * m_horizontal) + (v * m_vertical) - m_origin);
	}

private:
	Point3 m_origin;
	Point3 m_lowerLeftCorner;
	Vec3 m_horizontal;
	Vec3 m_vertical;
};