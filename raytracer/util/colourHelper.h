#pragma once

#include "vec3.h"
#include "mathsHelper.h"

#include <iostream>

namespace ColourHelper {
	void writeColourOutput(std::ostream &out, Rgb pixelColour, int samplesPerPixel) {
        auto r = pixelColour.x();
        auto g = pixelColour.y();
        auto b = pixelColour.z();

        // Divide the color by the number of samples and gamma-correct for gamma=2.0.
        auto scale = 1.0 / samplesPerPixel;
        r = sqrt(scale * r);
        g = sqrt(scale * g);
        b = sqrt(scale * b);

        // Write the translated [0,255] value of each color component.
        out << static_cast<int>(256 * MathsHelper::clamp(r, 0.0, 0.999)) << ' '
            << static_cast<int>(256 * MathsHelper::clamp(g, 0.0, 0.999)) << ' '
            << static_cast<int>(256 * MathsHelper::clamp(b, 0.0, 0.999)) << '\n';
    }

    void colourPostprocess(Rgb &pixelColour, int samplesPerPixel) {
        auto r = pixelColour.x();
        auto g = pixelColour.y();
        auto b = pixelColour.z();

        // Divide the color by the number of samples and gamma-correct for gamma=2.0.
        auto scale = 1.0 / samplesPerPixel;
        r = sqrt(scale * r);
        g = sqrt(scale * g);
        b = sqrt(scale * b);

        pixelColour.m_e[0] = 256 * MathsHelper::clamp(r, 0.0, 0.999);
        pixelColour.m_e[1] = 256 * MathsHelper::clamp(g, 0.0, 0.999);
        pixelColour.m_e[2] = 256 * MathsHelper::clamp(b, 0.0, 0.999);
    }
}