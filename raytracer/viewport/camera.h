#pragma once

#include "util/mathsHelper.h"

class Camera {
public:
	Camera(Point3 from,
		Point3 to,
		Vec3 viewUp,
		double vFov,
		double aspectRatio) {
		// Calculate the height/width using field of view 
		auto theta = MathsHelper::degreesToRadians(vFov);
		auto h = std::tan(theta / 2);
		auto viewportHeight = 2.0 * h;
		auto viewportWidth = aspectRatio * viewportHeight;

		//Calculate orthonormal basis (u, v, w) for camera orientation 
		auto w = Vec3::unitVector(from - to);
		auto u = Vec3::unitVector(Vec3::cross(viewUp, w));
		auto v = Vec3::cross(w, u);

		m_origin = from;
		m_horizontal = viewportWidth * u;
		m_vertical = viewportHeight * v;
		m_lowerLeftCorner = m_origin - (m_horizontal / 2) - (m_vertical / 2) - w;
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