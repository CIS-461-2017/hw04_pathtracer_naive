#include "lambertbrdf.h"
#include <warpfunctions.h>

Color3f LambertBRDF::f(const Vector3f &wo, const Vector3f &wi) const
{
    //TODO
    return Color3f(0.f);
}

Color3f LambertBRDF::Sample_f(const Vector3f &wo, Vector3f *wi, const Point2f &u,
                        Float *pdf, BxDFType *sampledType) const
{
    //TODO
    return BxDF::Sample_f(wo, wi, u, pdf, sampledType);
}

float LambertBRDF::Pdf(const Vector3f &wo, const Vector3f &wi) const
{
    //TODO
    return BxDF::Pdf(wo, wi);
}
