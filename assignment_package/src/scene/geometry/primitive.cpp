#include "primitive.h"

bool Primitive::Intersect(const Ray &r, Intersection *isect) const
{
    if(!shape->Intersect(r, isect)) return false;
    isect->objectHit = this;
    // Make sure to create a BSDF for our point of intersection
    if(material)
    {
        material->ProduceBSDF(isect);
    }
    return true;
}

bool Primitive::ProduceBSDF(Intersection *isect) const
{
    if(material)
    {
        material->ProduceBSDF(isect);
        return true;
    }
    return false;
}

const AreaLight* Primitive::GetAreaLight() const
{
    return areaLight.get();
}


const Material* Primitive::GetMaterial() const
{
    return material.get();
}
