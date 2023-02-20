#pragma once

#include <cmath>
#include <iostream>
#include "mathsHelper.h"

using std::sqrt;

class Vec3 {
public:
	Vec3() : m_e{ 0 ,0, 0} {}
	Vec3(double e0, double e1, double e2) : m_e{e0, e1, e2} {}

	double x() const { return m_e[0]; }
	double y() const { return m_e[1]; }
	double z() const { return m_e[2]; }

	Vec3 operator-() const { return Vec3(-m_e[0], -m_e[1], -m_e[2]); }
	double operator[](int i) const { return m_e[i]; }
	double &operator[](int i) { return m_e[i]; }

	Vec3 &operator+=(const Vec3& v) {
		m_e[0] += v.m_e[0];
		m_e[1] += v.m_e[1];
		m_e[2] += v.m_e[2];
		return *this;
	}


	Vec3& operator*=(const double t) {
		m_e[0] *= t;
		m_e[1] *= t;
		m_e[2] *= t;
		return *this;
	}

	Vec3 &operator/=(const double t) {
		return *this *= 1 / t;
	}

	double length() const {
		return sqrt(lengthSquared());
	}

	double lengthSquared() const {
		return m_e[0] * m_e[0] + m_e[1] * m_e[1] + m_e[2] * m_e[2];
	}

	bool nearZero() const {
		// Return true if the vector is close to zero in all dimensions.
		static const auto s = 1e-8;
		return (fabs(m_e[0]) < s) 
			&& (fabs(m_e[1]) < s) 
			&& (fabs(m_e[2]) < s);
	}

	friend std::ostream &operator<<(std::ostream &out, const Vec3 &v) {
		return out << v.m_e[0] << v.m_e[1] << v.m_e[2];
	}

	friend Vec3 operator+(const Vec3 &v1, const Vec3 &v2) {
		return Vec3(v1.m_e[0] + v2.m_e[0], v1.m_e[1] + v2.m_e[1], v1.m_e[2] + v2.m_e[2]);
	}

	friend Vec3 operator+(const Vec3 &v1, double val) {
		return Vec3(v1.m_e[0] + val, v1.m_e[1] + val, v1.m_e[2] + val);
	}

	friend Vec3 operator-(const Vec3 &v1, const Vec3 &v2) {
		return Vec3(v1.m_e[0] - v2.m_e[0], v1.m_e[1] - v2.m_e[1], v1.m_e[2] - v2.m_e[2]);
	}

	friend Vec3 operator-(const Vec3 &v1, double val) {
		return Vec3(v1.m_e[0] - val, v1.m_e[1] - val, v1.m_e[2] - val);
	}

	friend Vec3 operator*(const Vec3 &v1, const Vec3 &v2) {
		return Vec3(v1.m_e[0] * v2.m_e[0], v1.m_e[1] * v2.m_e[1], v1.m_e[2] * v2.m_e[2]);
	}

	friend Vec3 operator*(double t, const Vec3 &v) {
		return Vec3(t * v.m_e[0], t * v.m_e[1], t * v.m_e[2]);
	}

	friend Vec3 operator*(const Vec3 &v, double t) {
		return t * v;
	}

	friend Vec3 operator/(Vec3 v, double t) {
		return (1 / t) * v;
	}

	static double dot(const Vec3 &v1, const Vec3 &v2) {
		return v1.m_e[0] * v2.m_e[0]
			+ v1.m_e[1] * v2.m_e[1]
			+ v1.m_e[2] * v2.m_e[2];
	}

	static Vec3 cross(const Vec3 &v1, const Vec3 &v2) {
		return Vec3(v1.m_e[1] * v2.m_e[2] - v1.m_e[2] * v2.m_e[1],
			v1.m_e[2] * v2.m_e[0] - v1.m_e[0] * v2.m_e[2],
			v1.m_e[0] * v2.m_e[1] - v1.m_e[1] * v2.m_e[0]);
	}

	static Vec3 unitVector(Vec3 v) {
		return v / v.length();
	}

	static Vec3 random(double min, double max) {
		return Vec3(MathsHelper::randomDouble(min, max),
			MathsHelper::randomDouble(min, max),
			MathsHelper::randomDouble(min, max));
	}

	static Vec3 randomInUnitSphere() {
		Vec3 p;
		do {
			p = Vec3(MathsHelper::randomDouble(-1, 1),
				MathsHelper::randomDouble(-1, 1),
				MathsHelper::randomDouble(-1, 1));
		} while (p.lengthSquared() >= 1);
		return p;
	}

	static Vec3 randomUnitVector() {
		return unitVector(randomInUnitSphere());
	}

	static Vec3 randomInHemisphere(const Vec3 &normal) {
		Vec3 inUnitSphere = randomInUnitSphere();
		if (dot(inUnitSphere, normal) > 0.0) // In the same hemisphere as the normal
			return inUnitSphere;
		else
			return -inUnitSphere;
	}

	static Vec3 reflect(const Vec3 &v, const Vec3 &n) {
		// reflected ray = v + 2b
		// v = ray vector
		// b = y of v
		return v - 2 * dot(v, n) * n;
	}

	static Vec3 refract(const Vec3 &uv, const Vec3 &n, double refractionRatio) {
		// snell's law
		// sin n1 = n/n1 * sin n
		auto cosTheta = std::fmin(dot(-uv, n), 1);
		Vec3 rPerpendicular = refractionRatio * (uv + cosTheta * n);
		Vec3 rParallel = -std::sqrt(std::fabs(1.0 - rPerpendicular.lengthSquared())) * n;
		return rPerpendicular + rParallel;
	}
public:
	double m_e[3];
};

using Point3 = Vec3;
using Rgb = Vec3;
