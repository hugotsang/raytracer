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

	double degreesToRadians(double degrees);

	double randomDouble();

	double randomDouble(double min, double max);

	double clamp(double x, double min, double max);
}

