#include <iostream>
#include "util/mathsHelper.h"
#include "util/colourHelper.h"
#include "shapes/sphere.h"
#include "shapes/volumeList.h"
#include "viewport/camera.h"

Vec3 random(double min, double max) {
    return Vec3(MathsHelper::randomDouble(min, max),
                MathsHelper::randomDouble(min, max),
                MathsHelper::randomDouble(min, max));
}

Vec3 randomInUnitSphere() {
    Vec3 p;
    do {
        p = Vec3(MathsHelper::randomDouble(-1, 1),
                    MathsHelper::randomDouble(-1, 1),
                    MathsHelper::randomDouble(-1, 1));
    } while(p.lengthSquared() >= 1);
    return p;
}

Vec3 randomUnitVector() {
    return unitVector(randomInUnitSphere());
}

Vec3 randomInHemisphere(const Vec3 &normal) {
    Vec3 inUnitSphere = randomInUnitSphere();
    if (dot(inUnitSphere, normal) > 0.0) // In the same hemisphere as the normal
        return inUnitSphere;
    else
        return -inUnitSphere;
}

Rgb rayColour(const Ray& r, const Volume & world, int depth) {
    HitRecord rec;
    if (depth <= 0)
        return Rgb(0, 0, 0);

    if (world.hit(r, 0.001, MathsHelper::s_infinity, rec)) {
        //Point3 target = rec.p + rec.normal + randomUnitVector();
        Point3 target = rec.p + randomInHemisphere(rec.normal);
        return 0.5 * rayColour(Ray(rec.p, target - rec.p), world, depth - 1);
    }
    Vec3 unitDirection = unitVector(r.direction());
    auto t = 0.5 * (unitDirection.y() + 1.0);
    return (1.0 - t) * Rgb(1.0, 1.0, 1.0) + t * Rgb(0.5, 0.7, 1.0);
}

int main() {

    // Image
    const auto aspectRatio = 16.0 / 9.0;
    const int imageWidth = 400;
    const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
    const int samplesPerPixel = 100;
    const int maxDepth = 20;

    // World
    VolumeList world;
    world.add(std::make_shared<Sphere>(Point3(0, 0, -1), 0.5));
    world.add(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100));

    // Camera
    Camera cam;

    // Render
    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    for (int j = imageHeight - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining" << j << ' ' << std::flush;
        for (int i = 0; i < imageWidth; ++i) {
            Rgb pixelColour(0, 0, 0);
            for (int s = 0; s < samplesPerPixel; ++s) {
                auto u = (i + MathsHelper::randomDouble()) / (imageWidth - 1);
                auto v = (j + MathsHelper::randomDouble()) / (imageHeight - 1);
                Ray r = cam.getRay(u, v);
                pixelColour += rayColour(r, world, maxDepth);
            }
            ColourHelper::writeColour(std::cout, pixelColour, samplesPerPixel);
        }
    }

    std::cerr << "\nDone\n";
}