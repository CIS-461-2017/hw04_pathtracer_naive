#pragma once
#include <globals.h>
#include <vector>

class Film{
public:
    Film();
    Film(unsigned int width, unsigned int height);

    void SetDimensions(unsigned int w, unsigned int h);
    void SetPixelColor(const Point2i& pixel, const Color3f pixelColor);
    Color3f GetColor(const Point2i& pixel);
    void WriteImage(const std::string &path);
    void WriteImage(QString path);
    Bounds2i bounds;

private:
    std::vector<std::vector<Color3f>> pixels;//A 2D array of pixels in which we can store colors
};
