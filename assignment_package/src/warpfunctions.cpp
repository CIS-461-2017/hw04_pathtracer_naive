#define _USE_MATH_DEFINES
#include "warpfunctions.h"
#include <math.h>

Point3f WarpFunctions::squareToDiskUniform(const Point2f &sample)
{
    //TODO
    throw std::runtime_error("You haven't yet implemented uniform disk warping!");
}

Point3f WarpFunctions::squareToDiskConcentric(const Point2f &sample)
{
    //TODO
    throw std::runtime_error("You haven't yet implemented concentric disk warping!");
}

float WarpFunctions::squareToDiskPDF(const Point3f &sample)
{
    //TODO
    return 0;
}

Point3f WarpFunctions::squareToSphereUniform(const Point2f &sample)
{
    //TODO
    throw std::runtime_error("You haven't yet implemented uniform sphere warping!");
}

float WarpFunctions::squareToSphereUniformPDF(const Point3f &sample)
{
    //TODO
    return 0;
}

Point3f WarpFunctions::squareToSphereCapUniform(const Point2f &sample, float thetaMin)
{
    //TODO
    throw std::runtime_error("You haven't yet implemented sphere cap warping!");
}

float WarpFunctions::squareToSphereCapUniformPDF(const Point3f &sample, float thetaMin)
{
    //TODO
    return 0;
}

Point3f WarpFunctions::squareToHemisphereUniform(const Point2f &sample)
{
    //TODO
    throw std::runtime_error("You haven't yet implemented uniform hemisphere warping!");
}

float WarpFunctions::squareToHemisphereUniformPDF(const Point3f &sample)
{
    //TODO
    return 0;
}

Point3f WarpFunctions::squareToHemisphereCosine(const Point2f &sample)
{
    //TODO
    throw std::runtime_error("You haven't yet implemented cosine-weighted hemisphere warping!");
}

float WarpFunctions::squareToHemisphereCosinePDF(const Point3f &sample)
{
    //TODO
    return 0;
}
