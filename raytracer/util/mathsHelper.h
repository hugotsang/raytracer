#pragma once

#ifndef M_PI
#define _USE_MATH_DEFINES
#endif // ! M_PI

#include <cstdlib>
#include <cmath>
#include <limits>
#include <memory>
#include <math.h>

namespace MathsHelper {
	const double s_infinity = std::numeric_limits<double>::infinity();

	double degreesToRadians(double degrees) {
		return (degrees * M_PI) / 180;
	}

	double randomDouble() {
		// Returns a random real in [0,1).
		return rand() / (RAND_MAX + 1.0);
	}

	double randomDouble(double min, double max) {
		// Returns a random real in [min,max).
		return min + (max - min) * randomDouble();
	}

	inline double clamp(double x, double min, double max) {
		if (x < min)
			return min;
		if (x > max)
			return max;
		return x;
	}
}

