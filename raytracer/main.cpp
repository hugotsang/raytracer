#include "util/mathsHelper.h"
#include "util/colourHelper.h"
#include "util/userInput.h"
#include "shapes/sphere.h"
#include "shapes/volumeList.h"
#include "viewport/camera.h"
#include "viewport/displayBuffer.h"
#include "viewport/displayWindow.h"

// STD Header Files
#include <iostream>

// Windows Header Files
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>

constexpr int FRAMES_PER_SECOND = 60;

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
    return Vec3::unitVector(randomInUnitSphere());
}

Vec3 randomInHemisphere(const Vec3 &normal) {
    Vec3 inUnitSphere = randomInUnitSphere();
    if (Vec3::dot(inUnitSphere, normal) > 0.0) // In the same hemisphere as the normal
        return inUnitSphere;
    else
        return -inUnitSphere;
}

Rgb rayColour(const Ray& r, const Volume & world, int depth) {
    HitRecord rec;
    if (depth <= 0)
        return Rgb(0, 0, 0);

    if (world.hit(r, 0.001, MathsHelper::s_infinity, rec)) {
        Ray scattered;
        Rgb attenuation;
        if (rec.material->scatter(r, rec, attenuation, scattered))
            return attenuation * rayColour(scattered, world, depth - 1);
        return Rgb(0, 0, 0);
    }
    Vec3 unitDirection = Vec3::unitVector(r.direction());
    auto t = 0.5 * (unitDirection.y() + 1.0);
    return (1.0 - t) * Rgb(1.0, 1.0, 1.0) + t * Rgb(0.5, 0.7, 1.0);
}

void appEntry() {
    // Image
    const int samplesPerPixel = 100;
    const int maxDepth = 20;

    // World
    VolumeList world;

    auto material_ground = std::make_shared<Lambertian>(Rgb(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<Lambertian>(Rgb(0.7, 0.3, 0.3));
    auto material_left = std::make_shared<Metal>(Rgb(0.8, 0.8, 0.8));
    auto material_right = std::make_shared<Metal>(Rgb(0.8, 0.6, 0.2));

    world.add(std::make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(std::make_shared<Sphere>(Point3(0.0, 0.0, -1.0), 0.5, material_center));
    world.add(std::make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(std::make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, material_right));

    // Camera
    Camera cam;

    // Render
    // ppm file Header
    //std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";
    const auto imageWidth = DisplayBuffer::instance().getWidth();
    const auto imageHeight = DisplayBuffer::instance().getHeight();

    DisplayWindow::instance().drawWindow();

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
            ColourHelper::writeColourOutput(std::cout, pixelColour, samplesPerPixel);
            DisplayWindow::instance().drawPixel(pixelColour);
        }
        DisplayWindow::instance().drawWindow();
    }
    std::cerr << "\nDone\n";
}

bool appUpdate(float elapsedTime) {
    return UserInput::instance().KeyDown(VK_ESCAPE);
}

int appExit() {

    return 0;
}

int main() {

    bool quit = false;
    double elapsedTime = 0.0;
    LARGE_INTEGER now;

    // App dimensions
    const auto aspectRatio = 16.0 / 9.0;
    const int width = 400;
    const int height = static_cast<int>(width / aspectRatio);

    // Set up counters for timing the frame
    static LARGE_INTEGER lastDrawTime;
    LARGE_INTEGER frequency;
    QueryPerformanceCounter(&lastDrawTime);
    QueryPerformanceFrequency(&frequency);
    DisplayWindow::createInstance(width, height);
    DisplayBuffer::createInstance(width, height);

    appEntry();

    while (!quit) {
        if (!DisplayWindow::instance().processMessages()) {
            std::cerr << "closing window \n";
            return false;
        }
        
        do {
            QueryPerformanceCounter(&now);
            elapsedTime = (now.QuadPart - lastDrawTime.QuadPart) * 1000.0 / frequency.QuadPart;

        } while (elapsedTime < 1000.0f / FRAMES_PER_SECOND);

        quit = appUpdate(static_cast<float>(elapsedTime) / 1000.0f);
    }
    appExit();
    return 0;
}
